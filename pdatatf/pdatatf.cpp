#include "pdatatf.h"
#include <stdio.h>
#include "../type/uid.h"
#include "../dimse/cfind/cfindrq.h"
PDataTF::PDataTF(/* args */)
{
    pDataTFPDU = new PDataTFPDU();
}

PDataTF::~PDataTF()
{
    delete pDataTFPDU;
    pDataTFPDU = NULL;
}

PDataTFPDU *PDataTF::InitDefaultPDataTFPDU(CDIMSE *command, int commandlen, vector<DcmElement> dataset, int datasetlen, unsigned char presentationid)
{
    pDataTFPDU->headItem.pduType = 0x04;
    pDataTFPDU->headItem.reserve = 0x00;

    PDV commandpdv = InitCommandPresentationDataValue(command, commandlen);
    PresentationDataValueItem commandpdvitem = InitPresentationDataValueItem(1, commandpdv);
    pDataTFPDU->presentationDataValueItemList.push_back(commandpdvitem);

    PDV datasetdpdv = InitDataSetPresentationDataValue(dataset, datasetlen);
    PresentationDataValueItem datasetpdvitem = InitPresentationDataValueItem(1, datasetdpdv);
    pDataTFPDU->presentationDataValueItemList.push_back(datasetpdvitem);

    pDataTFPDU->headItem.pduLen = 0;
    for (int i=0; i<pDataTFPDU->presentationDataValueItemList.size(); i++)
    {
        pDataTFPDU->headItem.pduLen += pDataTFPDU->presentationDataValueItemList[i].itemLen + 
               sizeof(pDataTFPDU->presentationDataValueItemList[i].itemLen) + 
               sizeof(pDataTFPDU->presentationDataValueItemList[i].presentationID);
    }

    return pDataTFPDU;
}

PresentationDataValueItem PDataTF::InitPresentationDataValueItem(unsigned char presentationid, PDV pdv)
{
    PresentationDataValueItem presentationDataValueItem;

    presentationDataValueItem.itemLen = pdv.len;
    presentationDataValueItem.presentationID = presentationid;
    
    presentationDataValueItem.presentationDataValue = pdv.presentationDataValue;

    return presentationDataValueItem;
}

PDV PDataTF::InitCommandPresentationDataValue(CDIMSE *command, int commandlen)
{
    vector<DcmElement> commandlist;
    commandlist.push_back(command->groupLength);
    commandlist.push_back(command->affectedSOPClassUID);
    commandlist.push_back(command->commandField);
    commandlist.push_back(command->commandDataSetType);
    
    if(memcmp(command->affectedSOPClassUID.data.data, &CFindRQ_CommandType, sizeof(CFindRQ_CommandType)) == 0)
    {
        commandlist.push_back(((CFindRQDIMSE*)command)->messageID);
        commandlist.push_back(((CFindRQDIMSE*)command)->priority);
    }
    else if(memcmp(command->affectedSOPClassUID.data.data, &CFindRSP_CommandType, sizeof(CFindRSP_CommandType)) == 0)
    {}

    PDV pdv;
    PresentationDataValue presentationDataValue;

    presentationDataValue.messageControlHeader = 0b00000011;
    presentationDataValue.messageCommandOrDataSetFragment = commandlist;

    pdv.presentationDataValue = presentationDataValue;
    // int groupLength = 0;
    // char *lengthBuf = new char[command->groupLength.data.len];
    
    // // command tag is little implicit
    // for(int i=0; i<command->groupLength.data.len; i++)
    // {
    //     // groupLength &= (command->groupLength.data.data[i] << (((command->groupLength.data.len - 1) -i) * 8));
    //     lengthBuf[i] = command->groupLength.data.data[command->groupLength.data.len - 1 - i];
    // }
    // memcpy(&groupLength, lengthBuf, command->groupLength.data.len);
    pdv.len = sizeof(presentationDataValue.messageControlHeader) + commandlen;
    return pdv;
}

PDV PDataTF::InitDataSetPresentationDataValue(vector<DcmElement> dataset, int datasetlen)
{
    PDV pdv;
    PresentationDataValue presentationDataValue;
    // cfind dataset just need one pdv
    presentationDataValue.messageControlHeader = 0b00000010;
    presentationDataValue.messageCommandOrDataSetFragment = dataset;

    pdv.presentationDataValue = presentationDataValue;
    pdv.len = sizeof(presentationDataValue.messageControlHeader) + datasetlen;
    return pdv;
}