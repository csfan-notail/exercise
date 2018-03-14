/*
** file: outman.js 
** Public utility fuctions
** auth: Milk
**/

/* 所有函数返回值定义: 正确: 1, 失败: 0
/*---------------- 常用公共函数 ----------------*/
// 选中所选择的选项
function select_to_selected(select_id, selected_value)
{
	var loop;
	var obj;

	obj = document.getElementById(select_id);
	for(loop = 0; loop < obj.options.length; loop++)
	{
		if(obj.options[loop].value == selected_value)
		{
			obj.options[loop].selected=true;
			break;	
		}
	}
	return;
}

function select_to_selected_byname(select_name, selected_value, id)
{
	var loop;
	var obj;

	obj = document.getElementsByName(select_name)[id];
	for(loop = 0; loop < obj.options.length; loop++)
	{
		if(obj.options[loop].value == selected_value)
		{
			obj.options[loop].selected=true;
			break;	
		}
	}
	return;
}

function select_radio(radio_name, radio_value,num)
{
    var loop;
    var obj;

    for (loop = 0; loop < num; loop++){
        obj=document.getElementsByName(radio_name)[loop];
        if (radio_value == obj.value){
            obj.checked = true;
            break;
        }
    }
    return;
}


// 对input标签写值
function write_to_input(input_id, input_value)
{
	document.getElementById(input_id).value=input_value;
}

function write_to_input_byname(input_name, input_value, id)
{
	document.getElementsByName(input_name)[id].value=input_value;
}

function return_tag(tag_id)
{
    return document.getElementById(tag_id);
}

// 返回标签值
function return_tag_value(tag_id)
{
	var obj=document.getElementById(tag_id).value;
	return obj
}

function return_tag_value_byname(tag_name, id)
{
	var obj = document.getElementsByName(tag_name)[id].value;
	return obj;
}

// 返回标签长度
function return_tag_length(tag_id)
{
	var obj=document.getElementById(tag_id).value.length;
	return obj;
}

// 对标签 disable
function tag_disable(tag_id)
{
	document.getElementById(tag_id).disabled=true;
}

// 对标签 enabled
function tag_enabled(tag_id)
{
	document.getElementById(tag_id).disabled=false;
}

function tag_hidden(tag_id){
    document.getElementById(tag_id).style.display="none";
}

function tag_display(tag_id){
    document.getElementById(tag_id).style.display="block";
}

// select add option
function add_option(select_id, name, value)
{
	var obj = document.getElementById(select_id);
	obj.options.add(new Option(name, value));
	return;
}

// 对checkbox配置
function write_to_checkbox(tag_id, value)
{
	if("1"==value)
	{
		document.getElementById(tag_id).checked=true;
	}
	return;
}

function write_to_checkbox_byname(tag_name, value, id)
{
	if("1"==value)
	{
		document.getElementsByName(tag_name)[id].checked=true;
	}
	return;
}

function checkbox_set_value(tag_id)
{
	if(document.getElementById(tag_id).checked)
	{
		document.getElementById(tag_id).value="1";
	}
	else
	{
		document.getElementById(tag_id).value="0";
	}
	return;
}

function checkbox_set_value_byname(tag_name, id)
{
	if(document.getElementsByName(tag_name)[id].checked)
	{
		document.getElementsByName(tag_name)[id].value="1";
		return 1;
	}
	else
	{
		document.getElementsByName(tag_name)[id].value="0";
		return 0;
	}
}

function checkbox_selected(tag_name, id)
{
	if(document.getElementsByName(tag_name)[id].checked)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 返回配置结果
function configResult(result, configsuccess, configfailed)
{
	if("0" == result)
	{
		alert(configsuccess);
	}
	else if("-1" == result)
	{
		alert(configfailed);
	}
	return;
}

/*---------------- 网络常规检测 ----------------*/
// 检测MAC地址是否合法
function check_mac_addr(mac_addr, format)
{
	var arr_mac = new Array();
	var tmp_mac;
	var loop;
	
	arr_mac = mac_addr.split(format);
	if(6 != arr_mac.length)
	{
		return false;
	}

	for(loop = 0; loop < 6; loop++)
	{
		tmp_mac = parseInt(arr_mac[loop], 16);
		if(isNaN(tmp_mac))
		{
			return false;
		}
		if(tmp_mac < 0 || tmp_mac > 255)
		{
			return false;
		}
	}
	return true;
}

function check_mac_addrx(mac_addr, format, id)
{
	var arr_mac = new Array();
	var tmp_mac;
	var loop, loop1;
	var c;
	
	arr_mac = mac_addr.split(format);
	if(id != arr_mac.length)
	{
		return false;
	}

	for(loop = 0; loop < id; loop++)
	{
		if(2 != arr_mac[loop].length)
		{
			return false;
		}
		var partern1 = /[a-f]+$/;
		var partern2 = /[A-F]+$/;
		var ch1=arr_mac[loop].charAt(0);
		var ch2=arr_mac[loop].charAt(1);
		
		if (partern1.test(ch1) && partern2.test(ch2))
		{
            return false;
		}
		if (partern1.test(ch2) && partern2.test(ch1))
		{
            return false;
		}
		for(loop1 = 0; loop1 < arr_mac[loop].length; loop1++)
		{
			c = arr_mac[loop].toLowerCase().charAt(loop1);
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <='f') || (c >= 'A' && c <= 'F'))
			{
				continue;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}


// 检测IPv4地址是否合法
function check_ipv4_addr(ip_addr)
{
	var arr_ip = new Array();
	var tmp_ip;
	var loop;

	arr_ip = ip_addr.split(".");
	if(4 != arr_ip.length)
	{
		return false;
	}

	for(loop = 0; loop < 4; loop++)
	{
		if("" ==arr_ip[loop])
		{
			return false;
		}
		
		if(isNaN(arr_ip[loop]))
		{
			return false;
		}

		tmp_ip = parseInt(arr_ip[loop]);
		if(tmp_ip < 0 || tmp_ip > 255)
		{
			return false;
		}
	}

	return true;
}

// 检测IPv6地址是否合法
function check_ipv6_addr(ip_addr)
{
	var matchStr = "((([0-9a-f]{1,4}:){7}([0-9a-f]{1,4}|:))|(([0-9a-f]{1,4}:){6}(:[0-9a-f]{1,4}|((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9a-f]{1,4}:){5}(((:[0-9a-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3})|:))|(([0-9a-f]{1,4}:){4}(((:[0-9a-f]{1,4}){1,3})|((:[0-9a-f]{1,4})?:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9a-f]{1,4}:){3}(((:[0-9a-f]{1,4}){1,4})|((:[0-9a-f]{1,4}){0,2}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9a-f]{1,4}:){2}(((:[0-9a-f]{1,4}){1,5})|((:[0-9a-f]{1,4}){0,3}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(([0-9a-f]{1,4}:){1}(((:[0-9a-f]{1,4}){1,6})|((:[0-9a-f]{1,4}){0,4}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:))|(:(((:[0-9a-f]{1,4}){1,7})|((:[0-9a-f]{1,4}){0,5}:((25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)(\.(25[0-5]|2[0-4]\d|1\d\d|[1-9]?\d)){3}))|:)))(%.+)?\s*$";
    var ret = ip_addr.match(matchStr);
    if (ret) {
        return true;
    } else {
        return false;
    }	
}

//check url
function checkUrl(urlString)
{
	var strRegex="^((https|http|ftp|rtsp|mms)?://)"  
			+ "?(([0-9a-z_!~*'().&=+$%-]+: )?[0-9a-z_!~*'().&=+$%-]+@)?" // ftp的user@  
			+ "(([0-9]{1,3}\.){3}[0-9]{1,3}" // IP形式的URL- 199.194.52.184  
			+ "|" // 允许IP和DOMAIN（域名）  
			+ "([0-9a-z_!~*'()-]+\.)*" // 域名- www.  
			+ "([0-9a-z][0-9a-z-]{0,61})?[0-9a-z]\." // 二级域名  
			+ "[a-z]{2,6})" // first level domain- .com or .museum  
			+ "(:[0-9]{1,4})?" // 端口- :80  
			+ "((/?)|" // a slash isn't required if there is no file name  
			+ "(/[0-9a-z_!~*'().;?:@&=+$,%#-]+)+/?)$"; 

	var re=new RegExp(strRegex); 
	
	return re.test(urlString); 
}


//ipv4 address check
function checkIPv4(ip) 
{ 
    obj=ip;
    var exp=/^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/; 
    var reg = obj.match(exp); 
    if(reg==null) 
    { 
        return false;//不合法
    } 
    else 
    { 
        return true; //合法
    } 
}

//ipv4 submask address check
function checkMask(mask) 
{ 
    obj=mask; 
    var exp=/^(254|252|248|240|224|192|128|0)\.0\.0\.0|255\.(254|252|248|240|224|192|128|0)\.0\.0|255\.255\.(254|252|248|240|224|192|128|0)\.0|255\.255\.255\.(254|252|248|240|224|192|128|0)$/; 
    var reg = obj.match(exp); 
    if(reg==null) 
    { 
         return false; //"非法"
    } 
     else 
    { 
         return true; //"合法"
    } 
}    



/*---------------- 一般常规检测 ----------------*/
// 检测输入是否为空
function check_object_value(object_id)
{
	String.prototype.Trim = function(){ 
	return this.replace(/^\s+|\s+$/g,""); 
	}
	if("" == document.getElementById(object_id).value.Trim())
	{
		return false;
	}
	return true;
}

function checkchar(inputchar)
{
   var  choosechar=/^[^\u4e00-\u9fa5]+$/;  
   return (choosechar.test(inputchar));
}


// 检测是否为数字
function check_int(object)
{
	if(isNaN(object))
	{
		return false;
	}
	return true;
}

//检测是否为数字或字母
function check_int_letter(value) 
{
	var Regx = /^[A-Za-z0-9]*$/;
	if (Regx.test(value)) {
	    return true;
	}
	else 
	{
	    return false;
	}
}

function check_int_16(value) 
{
	var Regx = /^[A-Fa-f0-9]*$/;
	if (Regx.test(value)) {
	    return true;
	}
	else 
	{
	    return false;
	}
}

// 检测数值输入范围是否合法
// base 表示多少进制
function check_int_range(object, start, end, base)
{
	var tmp_ob;
	var tmp_start;
	var tmp_end;

	if(isNaN(object))
	{
		return false;
	}

	tmp_ob = parseInt(object, base);
	tmp_start = parseInt(start, base);
	tmp_end = parseInt(end, base);

	if(tmp_ob < start || tmp_ob > end)
	{
		return false;
	}
	return true;
}

//光标聚焦
function focus_on_by_id(id)
{
    document.getElementById(id).focus();
}

function compare_mac_addr(mac1,mac2)
{
    var macArr1 = mac1.split(":");
    var macArr2 = mac2.split(":");

    for (var i = 0; i < 6; i++){
        if (macArr1[i].toUpperCase() != macArr2[i].toUpperCase()){
            return false;
        }
    }
    return true;
}

/*---------------- 无线常规检测 ----------------*/
// 检测SSID输入是否合法



/*---------------- DHCP常规检测 ----------------*/
// 

/** end **/

function getauthority(name)
{
	var arr,reg=new RegExp("(^| )"+name+"=([^;]*)(;|$)");
	if(arr=document.cookie.match(reg))
		return unescape(arr[2]);
	else
		return null;
}
