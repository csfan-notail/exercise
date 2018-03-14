#
# host tools and environment quick install
#

APENV_PATH=/home/apenv/ta37/qsdk
APENV_PATH_BUILD_HOST=$(APENV_PATH)/build_dir/host
APENV_PATH_STAGING_HOST=$(APENV_PATH)/staging_dir/host
APENV_PATH_DL=$(APENV_PATH)/dl
APENV_SVN_DL=svn://10.1.1.101/openwrt/trunk/public/dl

define initenv/warning
	echo "######################################################################";	\
	echo "# LIYUGAO:WARNING: $(1) NOT EXIST!";	\
	echo "# Rebuilding host enviroment will take a long time!                  #";	\
	echo "# Please contact the administrator!!!                                #";	\
	echo "######################################################################"
endef
define initenv/touchref
	touch $(1)/reference;	\
	for x in `ls $(1) -A`;	\
	do	\
	  touch -r $(1)/reference $(1)/$${x};	\
	done
endef

.initenv:
	@echo "init env..."
	@#get dl
	@[ -d $(DL_DIR) ] || (	\
	mkdir -p $(DL_DIR);	\
	if [ -d $(APENV_PATH_DL) ]; then	\
	  echo "create links $(APENV_PATH_DL) --> $(DL_DIR)";	\
	  $(foreach f, $(wildcard $(APENV_PATH_DL)/*), ln -s $(f) $(DL_DIR)/; )	\
	  rm -rf $(DL_DIR)/busybox* ;	\
	  echo "rm -rf $(DL_DIR)/busybox*";	\
	  rm -rf $(DL_DIR)/quagga* ;	\
	  echo "rm -rf $(DL_DIR)/quagga*";	\
	  cp -a $(TOPDIR)/taudl/* $(DL_DIR) ;	\
	  echo "cp -a $(TOPDIR)/taudl/* $(DL_DIR)";	\
	else	\
	  echo "checkout $(APENV_SVN_DL) --> $(DL_DIR)";	\
	  svn co $(APENV_SVN_DL) $(DL_DIR);	\
	fi; )
	@#create host tools
	@([ -d $(STAGING_DIR_HOST) ] || [ -d $(BUILD_DIR_HOST) ]) || (	\
	if [ -d $(APENV_PATH_BUILD_HOST) ] && [ -d $(APENV_PATH_STAGING_HOST) ];then	\
	  echo "copy $(APENV_PATH_BUILD_HOST) --> $(BUILD_DIR_HOST)";	\
	  cp -fr $(APENV_PATH_BUILD_HOST) $(BUILD_DIR_HOST);	\
	  $(call initenv/touchref,$(BUILD_DIR_HOST)/stamp);	\
	  echo "copy $(APENV_PATH_STAGING_HOST) --> $(STAGING_DIR_HOST)";	\
	  cp -fr $(APENV_PATH_STAGING_HOST) $(STAGING_DIR_HOST);	\
	  $(call initenv/touchref,$(STAGING_DIR_HOST)/stamp);	\
	  touch $(tools/stamp-install);	\
	else	\
	  [ -d $(APENV_PATH_BUILD_HOST) ] || $(call initenv/warning, $(APENV_PATH_BUILD_HOST));	\
	  [ -d $(APENV_PATH_STAGING_HOST) ] || $(call initenv/warning, $(APENV_PATH_STAGING_HOST));	\
	fi; )	
	touch $@

