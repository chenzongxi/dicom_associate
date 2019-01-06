#ifndef PDUPDATATF_H
#define PDUPDATATF_H


#include <stdint.h>
#include <vector>
#include "../dimse/dimse.h"
using namespace std;


namespace PDUPDataTF_namespace
{
    struct HeadItem
    {
        unsigned char pduType;
        unsigned char reserve;
        uint32_t pduLen;
    };

    struct PresentationDataValue
    {
        unsigned char messageControlHeader;
        void *messageCommandOrDataSetFragment;
    };

    // PDV
    struct PresentationDataValueItem
    {
        uint32_t itemLen;
        unsigned char presentationID;

        PresentationDataValue presentationDataValue;
    };

    

    struct PDataTFPDU
    {
        HeadItem headItam;

        vector<PresentationDataValueItem> presentationDataValueItemList;

    };


}


















#endif // !CFIND_RQ_H