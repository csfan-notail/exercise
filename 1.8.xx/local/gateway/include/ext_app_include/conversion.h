/******************************************************************************

  Copyright (C), 2001-2011, GBCOM shanghai Co. Ltd.

 ******************************************************************************
  File Name     : conversion.h
  Version       : Initial Draft
  Author        : Zhou Shouya
  Created       : 2013/7/8
  Last Modified :
  Description   : conversion.c header file
  Function List :
  History       :
  1.Date        : 2013/7/8
    Author      : Zhou Shouya
    Modification: Created file

******************************************************************************/
#ifndef __CONVERSION_H__
#define __CONVERSION_H__

extern int restore_conversion(factory_restore *pst_restore, char *argv[]);
extern int vlan_info_conversion(vlan_info *pst_vlan, char *argv[]);
extern int vlan_mapping_conversion(vlan_map *pst_map, char *argv[]);

#endif /* __CONVERSION_H__ */
