/*
 * XRA1403 SPI/GPIO gpio expander driver
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/gpio.h>

#include <linux/spi/spi.h>
#include <linux/spi/xra1403.h>
#include <linux/slab.h>
#include <asm/byteorder.h>



struct xra1403;

struct xra1403_ops {
	int	(*read)(struct xra1403 *xra, unsigned reg);
	int	(*write)(struct xra1403 *xra, unsigned reg, unsigned val);
	int	(*read_regs)(struct xra1403 *xra, unsigned reg,
			     u16 *vals, unsigned n);
};

struct xra1403{
	u16			cache[COMMAND_NUM]; /*command cache */
	/* lock protects the cached values */
	struct mutex		lock;

	struct gpio_chip	chip;

	const struct xra1403_ops	*ops;
	void			*data; /* ops specific data */
};


struct xra1403_driver_data {
	unsigned		ngpio;
	struct xra1403	xra;
};

/*----------------------------------------------------------------------*/

static int xra1403_read(struct xra1403 *xra, unsigned reg)
{
	u8	tx[1], rx[1];
	int	status;
	
	tx[0] = reg << 1 | (COMMAND_RD << COMMAND_RDWR_SHIFT) ; /*Command Byte: no 1-6 bit  read-1/write-0  : no 7 bit */
	status = spi_write_then_read(xra->data, tx, sizeof tx, rx, sizeof rx);
	return (status < 0) ? status : (rx[0]);
}

static int xra1403_write(struct xra1403 *xra, unsigned reg, unsigned val)
{
	u8	tx[2];
	
	tx[0] = reg << 1;
	tx[1] = val;
	return spi_write_then_read(xra->data, tx, sizeof tx, NULL, 0);
}


static int
xra1403_read_regs(struct xra1403 *xra, unsigned reg, u16 *vals, unsigned n)
{
	u8	tx[2];
	int	status;
	tx[0] = reg << 1| (COMMAND_RD << COMMAND_RDWR_SHIFT) ;

	status = spi_write_then_read(xra->data, tx, sizeof tx,
				     (u8 *)vals, n * 2);
	if (status >= 0) {
		while (n--)
			vals[n] = __le16_to_cpu((__le16)vals[n]);
	}

	return status;
}

static const struct xra1403_ops xra1403_ops = {
	.read		= xra1403_read,
	.write		= xra1403_write,
	.read_regs	= xra1403_read_regs,
};



/*----------------------------------------------------------------------*/

static int xra1403_direction_input(struct gpio_chip *chip, unsigned offset)
{
	struct xra1403	*xra = container_of(chip, struct xra1403, chip);
	int status;

	if(offset < 0 || offset >=16)
		return 0;
	mutex_lock(&xra->lock);
	if(offset< 8){
		xra->cache[COMMAND_BYTE_GCR1] = xra1403_read(xra,COMMAND_BYTE_GCR1);
		xra->cache[COMMAND_BYTE_GCR1] |= (1 << offset);
		status = xra->ops->write(xra, COMMAND_BYTE_GCR1, xra->cache[COMMAND_BYTE_GCR1]);
	}
	else{
		offset -= 8;
		xra->cache[COMMAND_BYTE_GCR2] = xra1403_read(xra,COMMAND_BYTE_GCR2);
		xra->cache[COMMAND_BYTE_GCR2] |= (1 << (offset));
		status = xra->ops->write(xra, COMMAND_BYTE_GCR2, xra->cache[COMMAND_BYTE_GCR2]);
	}
	
	
	mutex_unlock(&xra->lock);
	return status;
}

static int xra1403_get(struct gpio_chip *chip, unsigned offset)
{
	struct xra1403	*xra = container_of(chip, struct xra1403, chip);
	int status;

	if(offset < 0 || offset >=16)
		return 0;

	mutex_lock(&xra->lock);

	if(offset< 8){
		status = xra->ops->read(xra, COMMAND_BYTE_GSR1);
		if (status < 0)
			status = 0;
		else {
			xra->cache[COMMAND_BYTE_GSR1] = status;
			status = !!(status & (1 << offset));
		}
	}
	else
	{
		status = xra->ops->read(xra, COMMAND_BYTE_GSR2);
		if (status < 0)
			status = 0;
		else {
			offset -= 8;
			xra->cache[COMMAND_BYTE_GSR2] = status;
			status = !!(status & (1 << (offset)));
		}
	}
	
	mutex_unlock(&xra->lock);
	return status;
}

static int __xra1403_set(struct xra1403 *xra, unsigned mask, int value,unsigned reg)
{
	unsigned outcache;
	xra->cache[reg] = xra1403_read(xra,reg);
	
	outcache = xra->cache[reg];
	if (value)
		outcache |= mask;
	else
		outcache &= ~mask;
	xra->cache[reg] = outcache;
	return xra->ops->write(xra, reg, outcache);
}

static void xra1403_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct xra1403	*xra = container_of(chip, struct xra1403, chip);
	unsigned mask;

	if(offset < 0 || offset >=16)
		return ;
	
	mutex_lock(&xra->lock);

	if(offset< 8){
		mask = 1 << offset;
		__xra1403_set(xra,mask,value,COMMAND_BYTE_OCR1);
	}
	else
	{
		offset -= 8;
		mask = 1 << offset;
		__xra1403_set(xra,mask,value,COMMAND_BYTE_OCR2);
	}
	
	mutex_unlock(&xra->lock);
	return;
}

static int
xra1403_direction_output(struct gpio_chip *chip, unsigned offset, int value)
{
	struct xra1403	*xra = container_of(chip, struct xra1403, chip);
	unsigned mask;
	int status;

	if(offset < 0 || offset >=16)
		return 0;
	mutex_lock(&xra->lock);
	if(offset< 8){
		mask = 1 << offset;
		xra->cache[COMMAND_BYTE_GCR1] = xra1403_read(xra,COMMAND_BYTE_GCR1);
		//printk("%s %d xra->cache[COMMAND_BYTE_GCR1] %x \n",__FUNCTION__,__LINE__,xra->cache[COMMAND_BYTE_GCR1]);
		xra->cache[COMMAND_BYTE_GCR1] &= ~mask;
		//printk("%s %d xra->cache[COMMAND_BYTE_GCR1] %x \n",__FUNCTION__,__LINE__,xra->cache[COMMAND_BYTE_GCR1]);
		status = xra->ops->write(xra, COMMAND_BYTE_GCR1, xra->cache[COMMAND_BYTE_GCR1]);
	}
	else
	{
		offset -= 8;
		mask = 1 << offset;
		xra->cache[COMMAND_BYTE_GCR2] = xra1403_read(xra,COMMAND_BYTE_GCR2);
		//printk("%s %d xra->cache[COMMAND_BYTE_GCR2] %x \n",__FUNCTION__,__LINE__,xra->cache[COMMAND_BYTE_GCR2]);
		xra->cache[COMMAND_BYTE_GCR2] &= ~mask;
		//printk("%s %d xra->cache[COMMAND_BYTE_GCR2] %x \n",__FUNCTION__,__LINE__,xra->cache[COMMAND_BYTE_GCR2]);
		status = xra->ops->write(xra, COMMAND_BYTE_GCR2, xra->cache[COMMAND_BYTE_GCR2]);
	}
	mutex_unlock(&xra->lock);
	return status;
}

/*----------------------------------------------------------------------*/

#ifdef CONFIG_DEBUG_FS

#include <linux/seq_file.h>

/*
 * This shows more info than the generic gpio dump code:
 * pullups, deglitching, open drain drive.
 */
static void xra1403_dbg_show(struct seq_file *s, struct gpio_chip *chip)
{
	return;
}

#else
#define xra1403_dbg_show	NULL
#endif

/*----------------------------------------------------------------------*/

static int xra1403_probe_one(struct xra1403 *xra, struct device *dev,
			      void *data, unsigned addr,
			      unsigned type, unsigned base)
{
	int status;

	mutex_init(&xra->lock);

	xra->data = data;

	xra->chip.direction_input = xra1403_direction_input;
	xra->chip.get = xra1403_get;
	xra->chip.direction_output = xra1403_direction_output;
	xra->chip.set = xra1403_set;
	xra->chip.dbg_show = xra1403_dbg_show;

	switch (type) {
	case 0:
		xra->ops = &xra1403_ops;
		xra->chip.ngpio = 16;
		xra->chip.label = "xra1403-0";
		break;
	case 1:
		xra->ops = &xra1403_ops;
		xra->chip.ngpio = 16;
		xra->chip.label = "xra1403-1";
		break;
	default:
		dev_err(dev, "invalid device type (%d)\n", type);
		return -EINVAL;
	}

	xra->chip.base = base;
	xra->chip.can_sleep = 1;
	xra->chip.dev = dev;
	xra->chip.owner = THIS_MODULE;

	/*register value init*/
	xra->cache[COMMAND_BYTE_GCR1] = 0xffff;
	xra->cache[COMMAND_BYTE_GCR2] = 0xffff;

	status = gpiochip_add(&xra->chip);
	if (status < 0)
		dev_dbg(dev, "can't setup chip %d, --> %d\n",
			addr, status);

	//printk("%s %d  base %d  \n",__FUNCTION__,__LINE__,xra->chip.base);
	return status;
}

/*----------------------------------------------------------------------*/



/*----------------------------------------------------------------------*/

static int xra1403_probe(struct spi_device *spi)
{
	struct xra1403_platform_data	*pdata;
	unsigned			addr;
	struct xra1403_driver_data	*data;
	int				status, type;

	unsigned	base;

	type = spi_get_device_id(spi)->driver_data;

	pdata = spi->dev.platform_data;

	if(!pdata)
		base = -1;

	data = kzalloc(sizeof(struct xra1403_driver_data),
			GFP_KERNEL);
	if (!data)
		return -ENOMEM;
	spi_set_drvdata(spi, data);

	status = xra1403_probe_one(&data->xra, &spi->dev, spi,
				    addr, type,base
				    );
	if (status < 0)
		goto fail;

	return 0;

fail:
	kfree(data);
	return status;
}

static int xra1403_remove(struct spi_device *spi)
{
	struct xra1403_driver_data	*data = spi_get_drvdata(spi);
	int				status = 0;
	int tmp;
	tmp = gpiochip_remove(&data->xra.chip);
	if (tmp < 0) {
		dev_err(&spi->dev, "%s --> %d\n", "remove", tmp);
		status = tmp;
	}
	
	if (status == 0)
		kfree(data);
	return status;
}

static const struct spi_device_id xra1403_ids[] = {
      { "xra1403-0", 0 },
      { "xra1403-1", 1 },
	{ },
};
MODULE_DEVICE_TABLE(spi, xra1403_ids);

static struct spi_driver xra1403_driver = {
	.probe		= xra1403_probe,
	.remove		= xra1403_remove,
	.id_table	= xra1403_ids,
	.driver = {
		.name	= "xra1403",
		.owner	= THIS_MODULE,
	},
};

static int __init xra1403_spi_init(void)
{
	return spi_register_driver(&xra1403_driver);
}

static void xra1403_spi_exit(void)
{
	spi_unregister_driver(&xra1403_driver);
}



/*----------------------------------------------------------------------*/

static int __init xra1403_init(void)
{
	int ret;

	ret = xra1403_spi_init();
	if (ret)
		goto spi_fail;


 spi_fail:
	return ret;
}
/* register after spi  postcore initcall and before
 * subsys initcalls that may rely on these GPIOs
 */
subsys_initcall(xra1403_init);

static void __exit xra1403_exit(void)
{
	xra1403_spi_exit();
}
module_exit(xra1403_exit);

MODULE_LICENSE("GPL");




