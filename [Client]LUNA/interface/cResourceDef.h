#ifndef _cRESOURCEHEADER_H_
#define _cRESOURCEHEADER_H_

#include "MHFile.h"

#define SWINDOWTYPE(a)		switch(a){

#define CASE(a)			case a:{
#define BREAK			}break;
#define EWINDOWTYPE			}


#define CMD_ST(a)	char buffer[255]; strcpy(buffer, a); if(0){
#define CMD_CS(b)		}else if(strcmp(buffer, b) == 0){
#define CMD_EN			}

#define FILE_IMAGE_MSG			"./Data/Interface/Windows/InterfaceMsg.bin"

// 070116 LYW --- Modified bin file name of chat_msg.bin file.
//#define FILE_CHAT_MSG			"./Data/Interface/Windows/chat_msg.bin"
#define FILE_CHAT_MSG			"./Data/Interface/Windows/SystemMsg.bin"

//// 070608 LYW --- cResourceDef : Delete emotion filter.
//// 070222 LYW --- cResourceDef : Define location and file name for filter message.
//#define FILE_CHATFILTER_MSG		"./Data/Interface/Windows/Chat_Filter.bin"
#define FILE_CHAT_FACELIST		"./Data/interface/Windows/Chat_FaceList.bin"

// 070910 LYW --- cResourceDef : Add action command.
#define FILE_ACTION_COMMAND		"./Data/interface/Windows/Action_Command.bin"

#define FILE_IMAGE_HARD_PATH	"./Data/Interface/Windows/image_hard_path.bin"
#define FILE_IMAGE_ITEM_PATH	"./Data/Interface/Windows/image_item_path.bin"
#define FILE_IMAGE_SKILL_PATH	"./Data/Interface/Windows/image_skill_path.bin"
// 070216 LYW --- cResourceDef : Define conduct image path.
#define FILE_IMAGE_CONDUCT_PACH	"./Data/Interface/Windows/Image_Conduct_Path.bin"

//070116 LYW --- Modified bin file name.
//#define FILE_IMAGE_BUFF_PATH	"./Data/Interface/Windows/image_Buff_path.bin"
#define FILE_IMAGE_BUFF_PATH	"./Data/Interface/Windows/image_Buff_path.bin"

// 070116 LYW --- Modified bin file name.
//#define FILE_IMAGE_MINIMAP_PATH	"./Data/Interface/Windows/mage_Minimap_path.bin"
#define FILE_IMAGE_MINIMAP_PATH	"./Data/Interface/Windows/Image_Minimap_Path.bin"

#define FILE_IMAGE_JACKPOT_PATH	"./Data/Interface/Windows/mage_jackpot_path.bin"
#define FILE_CHATTIP_MSG		"./Data/Interface/Windows/ChatTipMsg.bin"

#define	FILE_IMAGE_DATEMATCHING_PATH "./Data/Interface/Windows/image_DateMatching_path.bin"		// µ¥ÀÌÆ® ¸ÅÄª

// 070607 LYW --- cResourceDef : Define location and file name of favor icon list.
#define FILE_IMAGE_FAVOR_ICON_PATH	"./Data/Interface/Windows/image_favoritem_path.bin"

// 070714 LYW --- cResourceDef : Define location and file name of class icon list.
#define FILE_IMAGE_CLASS_ICON_PATH	"./Data/Interface/Windows/image_class_path.bin"
						
#endif //_cRESOURCEHEADER_H_
//
