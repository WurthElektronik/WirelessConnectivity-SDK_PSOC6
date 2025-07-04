/*
 ***************************************************************************************************
 * This file is part of WIRELESS CONNECTIVITY SDK:
 *
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
 * EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
 * TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
 * MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
 * WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
 * RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
 * COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
 * WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
 * FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
 * THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
 *
 * COPYRIGHT (c) 2025 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 */

/**
 * @file
 * @brief AT commands for SMS functionality.
 */
#include <stdio.h>

#include "ATSMS.h"
#include "Adrastea/AdrasteaI.h"
#include "ATCommands.h"


static const char* AdrasteaI_ATSMS_Message_State_Strings[AdrasteaI_ATSMS_Message_State_NumberOfValues] = {"REC UNREAD", "REC READ", "STO UNSENT", "STO SENT", "ALL"};

static const char* AdrasteaI_ATSMS_Storage_Location_Strings[AdrasteaI_ATSMS_Storage_Location_NumberOfValues] = {
    "BM", "ME", "MT", "SM", "TA", "SR",
};

/**
 * @brief Delete Message (using the AT+CMGD command).
 *
 * @param[in] index Message Index.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_DeleteMessage(AdrasteaI_ATSMS_Message_Index_t index)
{
    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CMGD=");

    if (!ATCommand_AppendArgumentInt(pRequestCommand, index, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_NOTATION_DEC), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, ATCOMMAND_CRLF, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief Delete All Message (using the AT+CMGD command).
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_DeleteAllMessages()
{
    if (!AdrasteaI_SendRequest("AT+CMGD=0,4\r\n"))
    {
        return false;
    }

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief List Messages (using the AT+CMGL command).
 *
 * @param[in] listType State of the messages to list.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ListMessages(AdrasteaI_ATSMS_Message_State_t listType)
{
    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CMGL=");

    if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, AdrasteaI_ATSMS_Message_State_Strings[listType], ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, ATCOMMAND_CRLF, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief Read Message (using the AT+CMGR command).
 *
 * @param[in] index Message Index.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ReadMessage(AdrasteaI_ATSMS_Message_Index_t index)
{
    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CMGR=");

    if (!ATCommand_AppendArgumentInt(pRequestCommand, index, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_NOTATION_DEC), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, ATCOMMAND_CRLF, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief Set Message Storage Locations (using the AT+CPMS command).
 *
 * @param[in] readDeleteStorage Storage for Read and Deleted Messages. See AdrasteaI_ATSMS_Storage_Location_t.
 *
 * @param[in] writeSendStorage Storage for Write and Send Messages (optional pass AdrasteaI_ATSMS_Storage_Location_Invalid to skip). See AdrasteaI_ATSMS_Storage_Location_t.
 *
 * @param[in] receiveStorage Storage for Received Messages (optional pass AdrasteaI_ATSMS_Storage_Location_Invalid to skip). See AdrasteaI_ATSMS_Storage_Location_t.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_SetMessageStorageLocations(AdrasteaI_ATSMS_Storage_Location_t readDeleteStorage, AdrasteaI_ATSMS_Storage_Location_t writeSendStorage, AdrasteaI_ATSMS_Storage_Location_t receiveStorage)
{
    AdrasteaI_optionalParamsDelimCount = 1;

    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CPMS=");

    if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, AdrasteaI_ATSMS_Storage_Location_Strings[readDeleteStorage], ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (writeSendStorage != AdrasteaI_ATSMS_Storage_Location_Invalid)
    {
        if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, AdrasteaI_ATSMS_Storage_Location_Strings[writeSendStorage], ATCOMMAND_ARGUMENT_DELIM))
        {
            return false;
        }
        AdrasteaI_optionalParamsDelimCount = 1;
    }
    else
    {
        if (!ATCommand_AppendArgumentString(pRequestCommand, ATCOMMAND_STRING_EMPTY, ATCOMMAND_ARGUMENT_DELIM))
        {
            return false;
        }
        AdrasteaI_optionalParamsDelimCount++;
    }

    if (receiveStorage != AdrasteaI_ATSMS_Storage_Location_Invalid)
    {
        if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, AdrasteaI_ATSMS_Storage_Location_Strings[receiveStorage], ATCOMMAND_STRING_TERMINATE))
        {
            return false;
        }
        AdrasteaI_optionalParamsDelimCount = 0;
    }

    pRequestCommand[strlen(pRequestCommand) - AdrasteaI_optionalParamsDelimCount] = ATCOMMAND_STRING_TERMINATE;

    if (!ATCommand_AppendArgumentString(pRequestCommand, ATCOMMAND_CRLF, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief Read Message Storage Usage (using the AT+CPMS command).
 *
 * @param[out] storageUsageP Storage Usage is returned in this argument. See AdrasteaI_ATSMS_Message_Storage_Usage_t.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ReadMessageStorageUsage(AdrasteaI_ATSMS_Message_Storage_Usage_t* storageUsageP)
{
    if (storageUsageP == NULL)
    {
        return false;
    }

    if (!AdrasteaI_SendRequest("AT+CPMS?\r\n"))
    {
        return false;
    }

    char* pResponseCommand = AT_commandBuffer;

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, pResponseCommand))
    {
        return false;
    }

    pResponseCommand += 1;

    if (!ATCommand_GetNextArgumentEnumWithoutQuotationMarks(&pResponseCommand, (uint8_t*)&storageUsageP->readDeleteStorageUsage.storageLocation, AdrasteaI_ATSMS_Storage_Location_Strings, AdrasteaI_ATSMS_Storage_Location_NumberOfValues, 30, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, &storageUsageP->readDeleteStorageUsage.usedMessages, ATCOMMAND_INTFLAGS_SIZE8 | ATCOMMAND_INTFLAGS_UNSIGNED, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, &storageUsageP->readDeleteStorageUsage.maxMessages, ATCOMMAND_INTFLAGS_SIZE8 | ATCOMMAND_INTFLAGS_UNSIGNED, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentEnumWithoutQuotationMarks(&pResponseCommand, (uint8_t*)&storageUsageP->writeSendStorageUsage.storageLocation, AdrasteaI_ATSMS_Storage_Location_Strings, AdrasteaI_ATSMS_Storage_Location_NumberOfValues, 30, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, &storageUsageP->writeSendStorageUsage.usedMessages, ATCOMMAND_INTFLAGS_SIZE8 | ATCOMMAND_INTFLAGS_UNSIGNED, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, &storageUsageP->writeSendStorageUsage.maxMessages, ATCOMMAND_INTFLAGS_SIZE8 | ATCOMMAND_INTFLAGS_UNSIGNED, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentEnumWithoutQuotationMarks(&pResponseCommand, (uint8_t*)&storageUsageP->receiveStorageUsage.storageLocation, AdrasteaI_ATSMS_Storage_Location_Strings, AdrasteaI_ATSMS_Storage_Location_NumberOfValues, 30, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, &storageUsageP->receiveStorageUsage.usedMessages, ATCOMMAND_INTFLAGS_SIZE8 | ATCOMMAND_INTFLAGS_UNSIGNED, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, &storageUsageP->receiveStorageUsage.maxMessages, ATCOMMAND_INTFLAGS_SIZE8 | ATCOMMAND_INTFLAGS_UNSIGNED, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}

/**
 * @brief Set Service Center Address (using the AT+CSCA command).
 *
 * @param[in] serviceCenterAddress Service Center Address.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_SetServiceCenterAddress(AdrasteaI_ATSMS_Service_Center_Address_t serviceCenterAddress)
{
    AdrasteaI_optionalParamsDelimCount = 1;

    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CSCA=");

    if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, serviceCenterAddress.address, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (serviceCenterAddress.addressType != AdrasteaI_ATSMS_Address_Type_Invalid)
    {
        if (!ATCommand_AppendArgumentInt(pRequestCommand, serviceCenterAddress.addressType, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_NOTATION_DEC), ATCOMMAND_STRING_TERMINATE))
        {
            return false;
        }
        AdrasteaI_optionalParamsDelimCount = 0;
    }

    pRequestCommand[strlen(pRequestCommand) - AdrasteaI_optionalParamsDelimCount] = ATCOMMAND_STRING_TERMINATE;

    if (!ATCommand_AppendArgumentString(pRequestCommand, ATCOMMAND_CRLF, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief Read Service Center Address (using the AT+CSCA command).
 *
 * @param[out] serviceCenterAddressP Service Center Address is returned in this argument.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ReadServiceCenterAddress(AdrasteaI_ATSMS_Service_Center_Address_t* serviceCenterAddressP)
{
    if (serviceCenterAddressP == NULL)
    {
        return false;
    }

    if (!AdrasteaI_SendRequest("AT+CSCA?\r\n"))
    {
        return false;
    }

    char* pResponseCommand = AT_commandBuffer;

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, pResponseCommand))
    {
        return false;
    }

    pResponseCommand += 1;

    if (!ATCommand_GetNextArgumentStringWithoutQuotationMarks(&pResponseCommand, serviceCenterAddressP->address, ATCOMMAND_ARGUMENT_DELIM, sizeof(serviceCenterAddressP->address)))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, &serviceCenterAddressP->addressType, ATCOMMAND_INTFLAGS_SIZE16 | ATCOMMAND_INTFLAGS_UNSIGNED, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}

/**
 * @brief Send Message (using the AT+CMGS command).
 *
 * @param[in] address Address.
 *
 * @param[in] addressType Address Type (optional pass AdrasteaI_ATSMS_Address_Type_Invalid to skip). See AdrasteaI_ATSMS_Address_Type_t.
 *
 * @param[in] message Message Payload.
 *
 * @param[out] messageReferenceP Message Reference is returned in this argument.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_SendMessage(AdrasteaI_ATSMS_Address_t address, AdrasteaI_ATSMS_Address_Type_t addressType, AdrasteaI_ATSMS_Message_Payload_t message, AdrasteaI_ATSMS_Message_Reference_t* messageReferenceP)
{
    if (messageReferenceP == NULL)
    {
        return false;
    }

    AdrasteaI_optionalParamsDelimCount = 1;

    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CMGS=");

    if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, address, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (addressType != AdrasteaI_ATSMS_Address_Type_Invalid)
    {
        if (!ATCommand_AppendArgumentInt(pRequestCommand, addressType, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_NOTATION_DEC), ATCOMMAND_STRING_TERMINATE))
        {
            return false;
        }
        AdrasteaI_optionalParamsDelimCount = 0;
    }

    pRequestCommand[strlen(pRequestCommand) - AdrasteaI_optionalParamsDelimCount] = ATCOMMAND_STRING_TERMINATE;

    if (!ATCommand_AppendArgumentString(pRequestCommand, "\r", ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, message, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, "\x1A", ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    char* pResponseCommand = AT_commandBuffer;

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, pResponseCommand))
    {
        return false;
    }

    while (*pResponseCommand != '\0' && *(pResponseCommand + 1) != '+')
    {
        pResponseCommand += 1;
    }

    pResponseCommand += 8;

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, messageReferenceP, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_SIZE8), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}

/**
 * @brief Send Message (using the AT+CMGSC command).
 *
 * @param[in] address Address.
 *
 * @param[in] addressType Address Type (optional pass AdrasteaI_ATSMS_Address_Type_Invalid to skip). See AdrasteaI_ATSMS_Address_Type_t.
 *
 * @param[in] message Message Payload.
 *
 * @param[out] messageReferenceP Message Reference is returned in this argument.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_SendLargeMessage(AdrasteaI_ATSMS_Address_t address, AdrasteaI_ATSMS_Address_Type_t addressType, AdrasteaI_ATSMS_Message_Payload_t message, AdrasteaI_ATSMS_Message_Reference_t* messageReferenceP)
{
    if (messageReferenceP == NULL)
    {
        return false;
    }

    AdrasteaI_optionalParamsDelimCount = 1;

    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT%CMGSC=");

    if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, address, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (addressType != AdrasteaI_ATSMS_Address_Type_Invalid)
    {
        if (!ATCommand_AppendArgumentInt(pRequestCommand, addressType, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_NOTATION_DEC), ATCOMMAND_STRING_TERMINATE))
        {
            return false;
        }
        AdrasteaI_optionalParamsDelimCount = 0;
    }

    pRequestCommand[strlen(pRequestCommand) - AdrasteaI_optionalParamsDelimCount] = ATCOMMAND_STRING_TERMINATE;

    if (!ATCommand_AppendArgumentString(pRequestCommand, "\r", ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, message, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, "\x1A", ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    char* pResponseCommand = AT_commandBuffer;

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, pResponseCommand))
    {
        return false;
    }

    while (*pResponseCommand != '\0' && *(pResponseCommand + 1) != '+')
    {
        pResponseCommand += 1;
    }

    pResponseCommand += 8;

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, messageReferenceP, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_SIZE8), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}

/**
 * @brief Write Message to Storage (using the AT+CMGW command).
 *
 * @param[in] address Address.
 *
 * @param[in] addressType Address Type (optional pass AdrasteaI_ATSMS_Address_Type_Invalid to skip). See AdrasteaI_ATSMS_Address_Type_t.
 *
 * @param[in] message Message Payload.
 *
 * @param[out] messageIndexP Message Index is returned in this argument.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_WriteMessageToStorage(AdrasteaI_ATSMS_Address_t address, AdrasteaI_ATSMS_Address_Type_t addressType, AdrasteaI_ATSMS_Message_Payload_t message, AdrasteaI_ATSMS_Message_Index_t* messageIndexP)
{
    if (messageIndexP == NULL)
    {
        return false;
    }

    AdrasteaI_optionalParamsDelimCount = 1;

    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CMGW=");

    if (!ATCommand_AppendArgumentStringQuotationMarks(pRequestCommand, address, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (addressType != AdrasteaI_ATSMS_Address_Type_Invalid)
    {
        if (!ATCommand_AppendArgumentInt(pRequestCommand, addressType, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_NOTATION_DEC), ATCOMMAND_STRING_TERMINATE))
        {
            return false;
        }
        AdrasteaI_optionalParamsDelimCount = 0;
    }

    pRequestCommand[strlen(pRequestCommand) - AdrasteaI_optionalParamsDelimCount] = ATCOMMAND_STRING_TERMINATE;

    if (!ATCommand_AppendArgumentString(pRequestCommand, "\r", ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, message, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, "\x1A", ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    char* pResponseCommand = AT_commandBuffer;

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, pResponseCommand))
    {
        return false;
    }

    while (*pResponseCommand != '\0' && *(pResponseCommand + 1) != '+')
    {
        pResponseCommand += 1;
    }

    pResponseCommand += 8;

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, messageIndexP, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_SIZE8), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}

/**
 * @brief Write Message to Storage (using the AT+CMGW command).
 *
 * @param[in] index Message Index.
 *
 * @param[out] messageReferenceP Message Index is returned in this argument.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_SendMessageFromStorage(AdrasteaI_ATSMS_Message_Index_t index, AdrasteaI_ATSMS_Message_Reference_t* messageReferenceP)
{
    if (messageReferenceP == NULL)
    {
        return false;
    }

    char* pRequestCommand = AT_commandBuffer;

    strcpy(pRequestCommand, "AT+CMSS=");

    if (!ATCommand_AppendArgumentInt(pRequestCommand, index, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_NOTATION_DEC), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!ATCommand_AppendArgumentString(pRequestCommand, ATCOMMAND_CRLF, ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    if (!AdrasteaI_SendRequest(pRequestCommand))
    {
        return false;
    }

    char* pResponseCommand = AT_commandBuffer;

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, pResponseCommand))
    {
        return false;
    }

    pResponseCommand += 1;

    if (!ATCommand_GetNextArgumentInt(&pResponseCommand, messageReferenceP, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_SIZE8), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}

/**
 * @brief Set SMS Notification Events (using the AT+CNMI command).
 *
 * @param[in] eventState Event State
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_SetSMSUnsolicitedNotificationEvents(AdrasteaI_ATCommon_Event_State_t eventState)
{
    switch (eventState)
    {
        case AdrasteaI_ATCommon_Event_State_Enable:
        {
            if (!AdrasteaI_SendRequest("AT+CNMI=2,1\r\n"))
            {
                return false;
            }
            break;
        }
        case AdrasteaI_ATCommon_Event_State_Disable:
        {
            if (!AdrasteaI_SendRequest("AT+CNMI=1,0\r\n"))
            {
                return false;
            }
            break;
        }
        default:
            return false;
    }

    if (!AdrasteaI_WaitForConfirm(AdrasteaI_GetTimeout(AdrasteaI_Timeout_SMS), AdrasteaI_CNFStatus_Success, NULL))
    {
        return false;
    }

    return true;
}

/**
 * @brief Parses the value of Read Message event arguments.
 *
 * @param[in]  pEventArguments String containing arguments of the AT command
 * @param[out] dataP SMS Message is returned in this argument. See AdrasteaI_ATSMS_Message_t.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ParseReadMessageEvent(char* pEventArguments, AdrasteaI_ATSMS_Message_t* dataP)
{
    if (dataP == NULL || pEventArguments == NULL)
    {
        return false;
    }

    char* argumentsP = pEventArguments;

    argumentsP += 1;

    dataP->messageIndex = 0;

    if (!ATCommand_GetNextArgumentEnumWithoutQuotationMarks(&argumentsP, (uint8_t*)&dataP->messageState, AdrasteaI_ATSMS_Message_State_Strings, AdrasteaI_ATSMS_Message_State_NumberOfValues, 30, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentStringWithoutQuotationMarks(&argumentsP, dataP->address, ATCOMMAND_ARGUMENT_DELIM, sizeof(dataP->address)))
    {
        return false;
    }

    if (ATCommand_CountArgs(argumentsP) == 3)
    {
        char extraData[40];

        if (!ATCommand_GetNextArgumentString(&argumentsP, extraData, ATCOMMAND_ARGUMENT_DELIM, sizeof(extraData)))
        {
            return false;
        }

        if (!ATCommand_GetNextArgumentStringWithoutQuotationMarks(&argumentsP, extraData, ATCOMMAND_ARGUMENT_DELIM, sizeof(extraData)))
        {
            return false;
        }
    }

    if (!ATCommand_GetNextArgumentString(&argumentsP, dataP->payload, ATCOMMAND_STRING_TERMINATE, sizeof(dataP->payload)))
    {
        return false;
    }

    return true;
}

/**
 * @brief Parses the value of List Message event arguments.
 *
 * @param[in]  pEventArguments String containing arguments of the AT command
 * @param[out] dataP SMS Message is returned in this argument. See AdrasteaI_ATSMS_Message_t.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ParseListMessagesEvent(char* pEventArguments, AdrasteaI_ATSMS_Message_t* dataP)
{
    if (dataP == NULL || pEventArguments == NULL)
    {
        return false;
    }

    char* argumentsP = pEventArguments;

    argumentsP += 1;

    if (!ATCommand_GetNextArgumentInt(&argumentsP, &dataP->messageIndex, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_SIZE8), ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentEnumWithoutQuotationMarks(&argumentsP, (uint8_t*)&dataP->messageState, AdrasteaI_ATSMS_Message_State_Strings, AdrasteaI_ATSMS_Message_State_NumberOfValues, 30, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentStringWithoutQuotationMarks(&argumentsP, dataP->address, ATCOMMAND_ARGUMENT_DELIM, sizeof(dataP->address)))
    {
        return false;
    }

    if (ATCommand_CountArgs(argumentsP) == 3)
    {
        char extraData[40];

        if (!ATCommand_GetNextArgumentString(&argumentsP, extraData, ATCOMMAND_ARGUMENT_DELIM, sizeof(extraData)))
        {
            return false;
        }

        if (!ATCommand_GetNextArgumentStringWithoutQuotationMarks(&argumentsP, extraData, ATCOMMAND_ARGUMENT_DELIM, sizeof(extraData)))
        {
            return false;
        }
    }

    if (!ATCommand_GetNextArgumentString(&argumentsP, dataP->payload, ATCOMMAND_STRING_TERMINATE, sizeof(dataP->payload)))
    {
        return false;
    }

    return true;
}

/**
 * @brief Parses the value of Message Received event arguments.
 *
 * @param[in]  pEventArguments String containing arguments of the AT command
 * @param[out] dataP SMS Message Received Result is returned in this argument. See AdrasteaI_ATSMS_Message_Received_Result_t.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ParseMessageReceivedEvent(char* pEventArguments, AdrasteaI_ATSMS_Message_Received_Result_t* dataP)
{
    if (dataP == NULL || pEventArguments == NULL)
    {
        return false;
    }

    char* argumentsP = pEventArguments;

    argumentsP += 1;

    if (!ATCommand_GetNextArgumentEnumWithoutQuotationMarks(&argumentsP, (uint8_t*)&dataP->storageLocation, AdrasteaI_ATSMS_Storage_Location_Strings, AdrasteaI_ATSMS_Storage_Location_NumberOfValues, 30, ATCOMMAND_ARGUMENT_DELIM))
    {
        return false;
    }

    if (!ATCommand_GetNextArgumentInt(&argumentsP, &dataP->messageIndex, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_SIZE8), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}

/**
 * @brief Parses the value of SMS Error event arguments.
 *
 * @param[in]  pEventArguments String containing arguments of the AT command
 * @param[out] dataP SMS Error is returned in this argument. See AdrasteaI_ATSMS_Error_t.
 *
 * @return true if successful, false otherwise
 */
bool AdrasteaI_ATSMS_ParseSMSErrorEvent(char* pEventArguments, AdrasteaI_ATSMS_Error_t* dataP)
{
    if (dataP == NULL || pEventArguments == NULL)
    {
        return false;
    }

    char* argumentsP = pEventArguments;

    argumentsP += 1;

    if (!ATCommand_GetNextArgumentInt(&argumentsP, dataP, (ATCOMMAND_INTFLAGS_UNSIGNED | ATCOMMAND_INTFLAGS_SIZE16), ATCOMMAND_STRING_TERMINATE))
    {
        return false;
    }

    return true;
}
