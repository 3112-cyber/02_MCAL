#include "GPIO.h"
/**
*	@file GPIO.h
*	@brief this header file will contain all required
*	definitions and basic utilities functions.
*	
*	@author FAREED
*
*	@date 2021-05-02
*/





//######################################### FUNCTION MARCROS #########################################
#define GETNUMBEROFCHANNELS(PORTID)                 (PortConfig[PORTID].ChannelNumbers)
#define GETPORTSTATE(PORTID)                        (*(PortConfig[PORTID].PortState))
#define SETPORTSTATE(PORTID,VALUE)                  ((GETPORTSTATE(PORTID)) = VALUE)                
#define GETPORTID(PORTID)                           (PortConfig[PORTID].PortId)
#define GETPORTVALUE(PORTID)                        (*(PortConfig[PORTID].PortValue))
#define SETPORTVALUE(PORTID,VALUE)                  ((GETPORTVALUE(PORTID)) = VALUE)                
#define GETCHANNELVALUE(PORTID,CHANNELID)           ((SFTR(GETPORTVALUE(PORTID),CHANNELID))&0x01)
#define GETPORTIDFROMCHANNELGROUPID(CHANNELGRPID)   (ChannelGroupConfig[CHANNELGRPID].PortId)
#define GETCHANNELGROUPMASK(CHANNELGRPID)           (ChannelGroupConfig[CHANNELGRPID].ChannelGroupMask)
#define GETCHANNELGROUPOFFSET(CHANNELGRPID)         (ChannelGroupConfig[CHANNELGRPID].ChannelGroupOffset)

#define WAITTILLPORTCOMPELETION(PORTID,VALUE)       do\
                                                        {\
                                                            \
                                                        }\
                                                      while(VALUE != GETPORTVALUE(PORTID))

#define WAITTILLSTATETRANSITION(PORTID,VALUE)       do\
                                                        {\
                                                            \
                                                        }\
                                                        while(VALUE != GETPORTSTATE(PORTID))
//####################################################################################################

//######################################### MARCROS ##################################################

//####################################################################################################

//######################################### EXTERN VARIABLES #########################################

//####################################################################################################

//######################################### GLOBAL VARIABLES #########################################

//####################################################################################################

//######################################### LOCAL FUNCTION ###########################################

//####################################################################################################

//######################################### GLOBAL FUNCTION ##########################################

//####################################################################################################


/**

*	Function description: This function is used to set port state 
*                        and initialize it by a certain value.

*	[In]  paramter type: "PortId" which is the port index.  
*                        "PortState" desired state for the port.
*                        "InitValue" initial value for the port.    

*	[Out] paramter type: None

*	Return Type: void

*	Authored by:

*/
void Dio_init(Dio_PortType PortId, uint8 PortState, uint8 InitValue)
{
    SETPORTSTATE(PortId, PortState);
    WAITTILLSTATETRANSITION(PortId,PortState);
    SETPORTVALUE(PortId, InitValue);
    WAITTILLPORTCOMPELETION(PortId,PortState);
}



/**

*	Function description: This function is used to read a certain pin value.

*	[In]  paramter type: "ChannelId" which is the pin index and 
*                        "PortId" which is the port index.  

*	[Out] paramter type: None

*	Return Type: "ChannelValue" Dio pin value.

*	Authored by:

*/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId, Dio_PortType PortId)
{
    Dio_LevelType ChannelValue = STD_ERROR;
    uint8 PortIterator; 
    for(PortIterator = 0; PortIterator < NUMBEROFPORTS(); PortIterator++)
    {
        if (PortId == GETPORTID(PortIterator)) 
        {
            if (ChannelId < GETNUMBEROFCHANNELS(PortId)) 
            {
                if(STD_HIGH == GETCHANNELVALUE(PortId,ChannelId))
                {
                    ChannelValue = STD_HIGH;
                }
                else if(STD_LOW == GETCHANNELVALUE(PortId,ChannelId))
                {
                    ChannelValue = STD_LOW;
                }
                else
                {
                    /*Do nothing*/
                }
            }
        }
    }
    return ChannelValue;
}

/**

*	Function description: This function is used to write a certain pin.

*	[In]  paramter type: "ChannelId" Channel Index,
*                        "Level" is the desired pin value.
*                        "PortId" Port Index.

*	[Out] paramter type: None

*	Return Type: None

*	Authored by:

*/

void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level, Dio_PortType PortId)
{
    uint8 PortIterator;
    for(PortIterator = 0; PortIterator < NUMBEROFPORTS(); PortIterator++) 
    {
        if (PortId == GETPORTID(PortIterator)) 
        {
            if (ChannelId < GETNUMBEROFCHANNELS(PortId)) 
            {
                SET_BIT_BY_VALUE(GETPORTVALUE(PortId), ChannelId, Level);
            }
        }
    }
}


/**

*	Function description: This Function is used to return the value of a certain
*                         port. 

*	[In]  paramter type:  "PortId" Port index.

*	[Out] paramter type: None

*	Return Type: "PortValue" holds the value of the port.

*	Authored by:

*/
uint8 Dio_ReadPort(Dio_PortType PortId)
{
    uint8 PortValue = 0x00;
    uint8 PortIterator;
    for(PortIterator = 0; PortIterator < NUMBEROFPORTS(); PortIterator++) 
    {
        if (PortId == GETPORTID(PortIterator)) 
        {
            PortValue = GETPORTVALUE(PortId);
        }
    }
    return PortValue;
}


/**

*	Function description: This Function is used to write the value of a certain
*                         port.

*	[In]  paramter type: "PortId" port index.
*                        "PortValue" the desired port value.

*	[Out] paramter type: None

*	Return Type: None

*	Authored by:

*/
void Dio_WritePort(Dio_PortType PortId, uint8 PortValue)
{
    uint8 PortIterator;
    for(PortIterator = 0; PortIterator < NUMBEROFPORTS(); PortIterator++) 
    {
        if (PortId == GETPORTID(PortIterator)) 
        {
            PortValue = SETPORTVALUE(PortId,PortValue);
        }
    }
}


/**

*	Function description: This function is used to read a group of channels in 
*                         one shot, note that the pin values are shifted to the 
*                         least significant bit. 

*	[In]  paramter type: "ChannelGroupId" the index of the configured channel 
*                        groups which is configured in "GPIO_Cfg.c"

*	[Out] paramter type: None

*	Return Type: "ChannelGrpValue" the value of the channel group shifted to the 
*                least significant bit. 

*	Authored by:

*/
uint8 Dio_ReadChannelGroup(uint8 ChannelGroupId)
{
    uint8 ChannelGrpValue = 0;
    uint8 PortInfoId = GETPORTIDFROMCHANNELGROUPID(ChannelGroupId);
    ChannelGrpValue = (GETPORTVALUE(PortInfoId) & GETCHANNELGROUPMASK(ChannelGroupId));
    ChannelGrpValue = SFTR(ChannelGrpValue,GETCHANNELGROUPOFFSET(ChannelGroupId));
    return ChannelGrpValue;
}

/**

*	Function description: This function is used to write a group of channels in 
*                         one shot, note that the pin values are shifted to the 
*                         most significant bit. 

*	[In]  paramter type: "ChannelGroupId" the index of the configured channel 
*                        groups which is configured in "GPIO_Cfg.c"
*                        "ChannelGrpValue" the provided channel group value passed
*                        from the upper layer. 

*	[Out] paramter type: None

*	Return Type: None

*	Authored by:

*/
void Dio_WriteChannelGroup(uint8 ChannelGroupId, uint8 ChannelGrpValue)
{
    uint8 SetBitPositions = 0u;
    uint8 PortInfoId = GETPORTIDFROMCHANNELGROUPID(ChannelGroupId);
    SetBitPositions = SFTL(ChannelGrpValue,GETCHANNELGROUPOFFSET(ChannelGroupId));
    SETPORTVALUE(PortInfoId,(SetBitPositions | GETPORTVALUE(PortInfoId)));
}

/**

*	Function description: This function is used to flip the channel state from 
*                         it's recent value to the inverted value. 

*	[In]  paramter type: "ChannelId" Channel Index,
*                        "PortId" Port Index.

*	[Out] paramter type: None

*	Return Type: None

*	Authored by:

*/
void Dio_FlipChannel(Dio_ChannelType ChannelId, Dio_PortType PortId)
{
    uint8 PinValue = 0;
    PinValue = GETPORTVALUE(PortId);
    PinValue = (SFTR(PinValue,ChannelId) & 0x01);
    PinValue = ~PinValue;
    SET_BIT_BY_VALUE((GETPORTVALUE(PortId)),ChannelId,PinValue);
}

