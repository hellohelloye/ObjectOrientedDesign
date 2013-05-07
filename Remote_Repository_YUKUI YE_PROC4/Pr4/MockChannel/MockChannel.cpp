///////////////////////////////////////////////////////////////////////
// MockChannel.cpp - echo messages from input queue to output queue  //
//                                                                   //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013         //
///////////////////////////////////////////////////////////////////////


#include "MockChannel.h"
#include "../Sender/Sender.h"

using namespace mockChannel;

/////////////////////////////////////////////////////////////////////////
// Helper message converters

typedef std::string stdMessage;

stdMessage ConvertMsgDown(Message^ msg)
{
  stdMessage smsg;
  for(int i=0; i<msg->Length; ++i)
    smsg += (int)msg[i];
  return smsg;
}

Message^ ConvertMsgUp(const stdMessage& smsg)
{
  Message^ msg;
  for(size_t i=0; i<smsg.size(); ++i)
    msg += (wchar_t)smsg[i];
  return msg;
}

typedef BlockingQueue<stdMessage> BQueue;

/////////////////////////////////////////////////////////////////////////
// Fawcett thread, used to make sure that will work with C++\CLI code

class Thread : public tthreadBase
{
public:
  Thread(BQueue* pInQ, BQueue* pOutQ) : pInQ_(pInQ), pOutQ_(pOutQ) {}
  void run()
  {
    while(true)
    {
      stdMessage msg = pInQ_->deQ();
      pOutQ_->enQ(msg);
      if(msg == "ChannelShutDown")
        break;
    }
  }
private:
  BQueue* pInQ_;
  BQueue* pOutQ_;
};

/////////////////////////////////////////////////////////////////////////
// ref class MockChannel echos messages from input queue to output queue

ref class MockChannel : public IChannel
{
public:
  MockChannel() : pInQ(new BQueue), pOutQ(new BQueue) 
  {
    Thread* pThread = new Thread(pInQ, pOutQ);
    pThread->start();
  }
  ~MockChannel()
  {
    delete pInQ;
    delete pOutQ;
  }

  virtual void shutDown() override
  {
    postMessage("ChannelShutDown");
  }

  virtual Message^ getMessage() override
  {
	stdMessage backmsg;
    stdMessage smsg = pOutQ->deQ();
	if(smsg.find("successfully")<smsg.length() || smsg.find("permitted")<smsg.length())
		backmsg = "succeed";
	else if(smsg.find("denied") < smsg.length() || smsg.find("wrong") < smsg.length())
		backmsg = "failed";
    return ConvertMsgUp(backmsg);
  }
  ////////////////////////////
  //forSendingMsgType: if send message choose = 1; if send file choose = 2;
  //loginWindow :  username*password&IP&SrcPort&RmtPort
  //sendXMLmetaDataWindow: username*versionNumber*dependency$filename&IP&SrcPort&RmtPort
  //sendFileWindow:  filename .cpp or .h or .txt
  //////////////////////////////////////
  virtual void postMessage(Message^ msg) override
  {
	std::string retmsg,sdoutmsg;
    stdMessage smsg = ConvertMsgDown(msg);
	if(smsg.find("*") < smsg.length() || smsg.find("&") < smsg.length())
		retmsg = channel->channelStart(smsg,1);
	else if(smsg.find(".h")<smsg.length() || smsg.find(".cpp")<smsg.length() || smsg.find(".txt"))
		retmsg = channel->channelStart(smsg,2);
    pInQ->enQ(retmsg);
  }

  void showQueues(const std::string& msg)
  {
    sout << locker << "\n  " << msg << unlocker;
    sout << locker << "\n inQ has " << pInQ->size() << " messages";
    sout << " and outQ has " << pOutQ->size() << " messages" << unlocker;
    sout.flush();
  }

private:
  myChannel* channel;
  BQueue* pInQ;
  BQueue* pOutQ;
};

IChannel^ IChannel::CreateChannel()
{
  return gcnew MockChannel();
}

int main()
{
  Message^ testMessage = "testMessage";
  stdMessage sTestMessage = ConvertMsgDown(testMessage);
  std::cout << "\n  " << sTestMessage;
  Message^ resultMessage = ConvertMsgUp(sTestMessage);
  Console::Write("\n  {0}\n", resultMessage);

  IChannel^ chan = IChannel::CreateChannel();
  Message^ msg = "a message";
  chan->postMessage(msg);
  Message^ retMsg = chan->getMessage();
  Console::Write("\n  {0}\n\n", retMsg);
  chan->shutDown();
}
