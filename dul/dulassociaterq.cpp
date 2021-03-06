#include "dulassociaterq.h"
#include <string.h>
#include <stdio.h>

AssociateRQDUL::AssociateRQDUL(string ip, int port)
{
    index = 0;

    tcpSocket = new TcpSocket();
    conn = tcpSocket->Connet(ip, port);
}

AssociateRQDUL::~AssociateRQDUL()
{
    delete tcpSocket;
    tcpSocket = NULL;
}

int AssociateRQDUL::DUL_sendAssociateRQ(AssociateRQPDU *associaterqpdu)
{
    DUL_GetAssociateRQPUDMemory(associaterqpdu);
    for(int i=0; i<this->associatedata.len; i++)
    {
        printf("%X: %d\n", this->associatedata.buffer[i], i);
    }
    tcpSocket->Send(conn, this->associatedata.buffer, this->associatedata.len);

    return conn;
}

void AssociateRQDUL::DUL_GetAssociateRQPUDMemory(AssociateRQPDU *associaterqpdu)
{
    int32_t associaterqpudheadlen = sizeof(associaterqpdu->pduHead.PduType) + sizeof(associaterqpdu->pduHead.Reserved) + sizeof(associaterqpdu->pduHead.PduLen);
    int32_t associaterqpudlen = associaterqpudheadlen + associaterqpdu->pduHead.PduLen;
    associatedata.len = associaterqpudlen;
    associatedata.buffer = new unsigned char[associatedata.len];
    memset(associatedata.buffer, 0, associatedata.len);

    //pdu head
    DUL_GetBufferFromPoint(&(associaterqpdu->pduHead.PduType), sizeof(associaterqpdu->pduHead.PduType));
    DUL_GetBufferFromPoint(&(associaterqpdu->pduHead.Reserved), sizeof(associaterqpdu->pduHead.Reserved));
    DUL_GetBufferFromInt(associaterqpdu->pduHead.PduLen, sizeof(associaterqpdu->pduHead.PduLen));

    DUL_GetBufferFromInt(associaterqpdu->ProtocolVersion, sizeof(associaterqpdu->ProtocolVersion));
    DUL_GetBufferFromInt(associaterqpdu->Reserved1, sizeof(associaterqpdu->Reserved1));
    DUL_GetBufferFromPoint(associaterqpdu->CalledAE, sizeof(associaterqpdu->CalledAE));
    DUL_GetBufferFromPoint(associaterqpdu->CallingAE, sizeof(associaterqpdu->CallingAE));
    DUL_GetBufferFromPoint(associaterqpdu->Reserved2, sizeof(associaterqpdu->Reserved2));

    DUL_GetApplicationContexItemMemory(&(associaterqpdu->applicationContexItem));
    for(int i=0; i<associaterqpdu->presentationContextItemlist.size();i++)
    {
        DUL_GetPresentationContextItemMemory(&(associaterqpdu->presentationContextItemlist[i]));
    }
    DUL_GetUserInfoItemItemMemory(&(associaterqpdu->userInfoItem));
}

void AssociateRQDUL::DUL_GetApplicationContexItemMemory(ApplicationContexItem *applicationcontexitem)
{
    DUL_GetBufferFromPoint(&(applicationcontexitem->itemHead.ItemType), sizeof(applicationcontexitem->itemHead.ItemType));
    DUL_GetBufferFromPoint(&(applicationcontexitem->itemHead.Reserved), sizeof(applicationcontexitem->itemHead.Reserved));
    DUL_GetBufferFromInt(applicationcontexitem->itemHead.ItemLen, sizeof(applicationcontexitem->itemHead.ItemLen));

    // convert &ApplicationContexItem to point cause applicationcontexitem->AppicationContextName changed
    // DUL_GetBufferFromPoint(applicationcontexitem->AppicationContextName.c_str(), applicationcontexitem->AppicationContextName.size());
    DUL_GetBufferFromPoint(applicationcontexitem->AppicationContextName, applicationcontexitem->itemHead.ItemLen);
}

void AssociateRQDUL::DUL_GetPresentationContextItemMemory(PresentationContextItem *presentationcontextitem)
{
    DUL_GetBufferFromPoint(&(presentationcontextitem->itemHead.ItemType), sizeof(presentationcontextitem->itemHead.ItemType));
    DUL_GetBufferFromPoint(&(presentationcontextitem->itemHead.Reserved), sizeof(presentationcontextitem->itemHead.Reserved));
    DUL_GetBufferFromInt(presentationcontextitem->itemHead.ItemLen, sizeof(presentationcontextitem->itemHead.ItemLen));

    DUL_GetBufferFromInt(presentationcontextitem->PresentationContextID, sizeof(presentationcontextitem->PresentationContextID));
    DUL_GetBufferFromPoint(presentationcontextitem->Reserved, sizeof(presentationcontextitem->Reserved));

    DUL_GetAbstractSyntaxMemory(&(presentationcontextitem->negotiationSyntaxItem.abstractSyntax));
    for(int j = 0;j < presentationcontextitem->negotiationSyntaxItem.transferSyntaxlist.size();j++)
    {
        DUL_GetTransferSyntaxMemory(&(presentationcontextitem->negotiationSyntaxItem.transferSyntaxlist[j]));
    }
}

void AssociateRQDUL::DUL_GetUserInfoItemItemMemory(UserInfoItem *userinfoitem)
{
    DUL_GetBufferFromPoint(&(userinfoitem->itemHead.ItemType), sizeof(userinfoitem->itemHead.ItemType));
    DUL_GetBufferFromPoint(&(userinfoitem->itemHead.Reserved), sizeof(userinfoitem->itemHead.Reserved));
    DUL_GetBufferFromInt(userinfoitem->itemHead.ItemLen, sizeof(userinfoitem->itemHead.ItemLen));

    DUL_GetMaximumLengthItemMemory(&(userinfoitem->maxLenItem));

}

void AssociateRQDUL::DUL_GetAbstractSyntaxMemory(SyntaxItem *abstractsyntaxitem)
{
    DUL_GetBufferFromPoint(&(abstractsyntaxitem->itemHead.ItemType), sizeof(abstractsyntaxitem->itemHead.ItemType));
    DUL_GetBufferFromPoint(&(abstractsyntaxitem->itemHead.Reserved), sizeof(abstractsyntaxitem->itemHead.Reserved));
    DUL_GetBufferFromInt(abstractsyntaxitem->itemHead.ItemLen, sizeof(abstractsyntaxitem->itemHead.ItemLen));

    DUL_GetBufferFromPoint(abstractsyntaxitem->Syntax, abstractsyntaxitem->itemHead.ItemLen);
}

void AssociateRQDUL::DUL_GetTransferSyntaxMemory(SyntaxItem *transfersyntaxitem)
{
    DUL_GetBufferFromPoint(&(transfersyntaxitem->itemHead.ItemType), sizeof(transfersyntaxitem->itemHead.ItemType));
    DUL_GetBufferFromPoint(&(transfersyntaxitem->itemHead.Reserved), sizeof(transfersyntaxitem->itemHead.Reserved));
    DUL_GetBufferFromInt(transfersyntaxitem->itemHead.ItemLen, sizeof(transfersyntaxitem->itemHead.ItemLen));

    DUL_GetBufferFromPoint(transfersyntaxitem->Syntax, transfersyntaxitem->itemHead.ItemLen);
}

void AssociateRQDUL::DUL_GetMaximumLengthItemMemory(MaximumLengthItem *maximumlengthitem)
{
    DUL_GetBufferFromPoint(&(maximumlengthitem->itemHead.ItemType), sizeof(maximumlengthitem->itemHead.ItemType));
    DUL_GetBufferFromPoint(&(maximumlengthitem->itemHead.Reserved), sizeof(maximumlengthitem->itemHead.Reserved));
    DUL_GetBufferFromInt(maximumlengthitem->itemHead.ItemLen, sizeof(maximumlengthitem->itemHead.ItemLen));

    DUL_GetBufferFromInt(maximumlengthitem->MaxLenReceived, sizeof(maximumlengthitem->MaxLenReceived));
}

void AssociateRQDUL::DUL_GetBufferFromPoint(const unsigned char *data, int len)
{
    memcpy(this->associatedata.buffer + index, data, len);
    index += len;
}

void  AssociateRQDUL::DUL_GetBufferFromInt(int data, int len)
{
    // 大端排列,scp服务端按照长度截取,直接转换为内存中的整形值.
    int pos = len - 1;
    for(int i=0; i<len; i++)
    {
        
        this->associatedata.buffer[index+pos] = (data & 0xFF);
        data = data >> 8;
        pos--;
    }
    index += len;
}