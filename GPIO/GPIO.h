#ifndef H_GPIO_H
#define H_GPIO_H
#include "..\..\03_Peripherals_Confguration\STD_Types.h"
//######################################### FUNCTION MARCROS #########################################
#define SFTR(REG,num) (REG>>num)
#define SFTL(REG,num) (REG<<num)
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )
#define SET_BIT_BY_VALUE(REG,BIT,VALUE) ((REG & ~(1 << BIT)) | (VALUE << BIT))
//####################################################################################################

//######################################### MARCROS ##################################################
#define PORTA_VALUE           ((volatile const uint8*) 0x0005)
#define PORTC_VALUE           ((volatile const uint8*) 0x0007)
#define TRISA_VALUE           ((volatile const uint8*) 0x0085)
#define TRISC_VALUE           ((volatile const uint8*) 0x0087)
#define NUMBEROFPORTS()   2u    
//####################################################################################################

//######################################### ENUMS ####################################################
typedef enum
{
    STD_ERROR = -1,
    STD_LOW  = 0,
    STD_HIGH = 1
}Dio_LevelType;

typedef enum
{
    CHANNEL_0 = 0,
    CHANNEL_1 = 1,
    CHANNEL_2 = 2,
    CHANNEL_3 = 3,
    CHANNEL_4 = 4,
    CHANNEL_5 = 5,
    CHANNEL_6 = 6,
    CHANNEL_7 = 7
}Dio_ChannelType;

typedef enum {
    PORT_A = 0,
    PORT_C = 1
}Dio_PortType;
//####################################################################################################

//######################################### STRUCTURES ###############################################
typedef struct
{
    uint8 PortId;
    uint8 *PortValue;
    uint8 *PortState;
    uint8 ChannelNumbers;
}PortConfigType;

typedef struct
{
    uint8 PortId; //n:1 relationship with PortConfig
    uint8 ChannelGroupMask;
    uint8 ChannelGroupOffset; //offset from the nearest bit to lsb. 
}ChannelGroupConfigType;
//####################################################################################################

//######################################### EXTERN VARIABLES #########################################
extern PortConfigType PortConfig[2];
extern ChannelGroupConfigType ChannelGroupConfig[4];
//####################################################################################################

//######################################### GLOBAL FUNCTION ##########################################
void Dio_init(Dio_PortType PortId, uint8 PortState, uint8 InitValue);
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId, Dio_PortType PortId);
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level, Dio_PortType PortId);
uint8 Dio_ReadPort(Dio_PortType PortId);
void Dio_WritePort(Dio_PortType PortId, uint8 PortValue);
uint8 Dio_ReadChannelGroup(uint8 ChannelGroupId);
void Dio_WriteChannelGroup(uint8 ChannelGroupId, uint8 ChannelGrpValue);
void Dio_FlipChannel(Dio_ChannelType ChannelId, Dio_PortType PortId);
//####################################################################################################
#endif
 