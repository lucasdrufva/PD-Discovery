#include "USB-PD.h"

PD_Engine pd;

void handleMessageCB(PD::Message msg)
{
    pd.handleMessage(msg);
}

void registerMessageCB(TcpmDriver *tcpm)
{
    tcpm->setReceivedMessageCB(&handleMessageCB);
}

uint8_t PD::parseHeaderMessageType(uint16_t header)
{
    return header & 0xF;
}

void PD_Engine::begin(TcpmDriver *tcpm)
{
    this->tcpm = tcpm;
    tcpm->begin();
    registerMessageCB(tcpm);
}

void PD_Engine::configureAsSource()
{
    this->tcpm->setIsSource(true);

    //is this needed?
    //Seems to prevent bugs sometimes
    //tcpm->resetPD();
}

void PD_Engine::configureAsSink()
{
    tcpm->setIsSource(false);
}

PD_Engine::Error PD_Engine::requestIdentity(PD::Destination dest, void (*cb)(PD::Identity))
{
    this->identityCB = cb;

    // //TODO use TCPM_DETECTION_RA
    // // if (!(tcpm->getConnection() & (1 << 3)))
    // // {
    // //     return PD_Engine::Error::DISCONNECT;
    // // }

    uint32_t VDM_header = 0;
    VDM_header |= PD::VDM::DISCOVER_IDENTITY;
    VDM_header |= PD::VDM::COMMAND_TYPE_REQ;
    VDM_header |= PD::VDM::STRUCTURED;
    VDM_header |= PD::VDM::STRUCTURED_VERSION;
    VDM_header |= PD::VDM::POWER_DELIVERY_SVID;

    uint16_t header = 0;

    //Message type: VDM
    header |= 0x0F;
    //Revision
    header |= 1 << 6;
    //Cable plug role
    header |= 0 << 8;
    //Message id
    header |= (this->tcpm->getMessageID() & 0x07) << 9;
    //Number of data objects
    header |= 1 << 12;

    this->tcpm->sendMessage(header, &VDM_header, dest);

    return PD_Engine::Error::NONE;
}

PD_Engine::Error PD_Engine::requestSourceCap(void (*cb)(PD::Capabilities))
{
    this->capabilitiesCB = cb;
}

void PD_Engine::registerCapCB(void (*cb)(PD::Capabilities))
{
    this->capabilitiesCB = cb;
}

void PD_Engine::handleMessage(PD::Message msg)
{
    // this->capabilitiesCB(msg.dataMessage.sourceCap);
    if (msg.type == PD::MessageType::DATA)
    {
        if (msg.dataMessage.type == PD::DataMessageType::VDM_TYPE)
        {
            if (msg.dataMessage.vdm.type == PD::VdmType::IDENTITY)
            {
                this->identityCB(msg.dataMessage.vdm.id);
            }
        }
        else if (msg.dataMessage.type == PD::DataMessageType::SOURCE_CAP)
        {
            this->capabilitiesCB(msg.dataMessage.sourceCap);
        }
    }
}