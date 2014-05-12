/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * See COPYING for copyright and distribution information.
 */

// correct way to include NDN-CPP headers
 #include <ndn-cxx/face.hpp>
 #include <ndn-cxx/security/key-chain.hpp>
 #include <stdlib.h>
 #include <ndn-cxx/util/command-interest-validator.hpp>
 #include <ndn-cxx/util/command-interest-generator.hpp>
//#include "face.hpp"
//#include "security/key-chain.hpp"

using namespace ndn;

class Producer
{
public:
  Producer()
  {
    for(int i=0;i<10;i++)
    {
    hasvalue[i]=false;
    ele[i]=0;
    }
    ID_num=0;
  }

  void
  onInterest( const Name& name, const Interest& interest)
  {
    // std::cout << "<< I: " << interest << std::endl;
    // Name alice("/ndn/test/alice");
    // Name alice1("/ndn/test/alice1");
    // validator_.addInterestRule("^<>",
    //                         *keyChain_.getCertificate(keyChain_.getDefaultCertificateNameForIdentity(alice)));
    // validator_.addInterestRule("^<>",
    //                         *keyChain_.getCertificate(keyChain_.getDefaultCertificateNameForIdentity(alice1)));
    // validator_.validate(interest,
    //                         ndn::bind(&Producer::onValidated, this, _1),
    //                         ndn::bind(&Producer::onValidationFailed, this, _1, _2));
    std::string command=ndn::Name(interest.getName()).get(2).toUri();
    if(command=="founded")
    {std::cout << "found" << std::endl;
      ID[ID_num]=ndn::Name(interest.getName()).get(3).toUri();
      ID_num++;
    }
    else if(command=="read")
    {

      num=std::atoi(ndn::Name(interest.getName()).get(3).toUri().c_str());
      if(hasvalue[num]==true)
      {
        ndn::Data data(ndn::Name(interest.getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        sprintf(result,"%f",ele[num]);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

        std::cout << ">> D: " << data << std::endl;
        face_.put(data);
      }
      else
      {
        for(int i=0; i<ID_num; i++) 
        {
          ndn::Name name("/ndn-caculator");
          char str[15];
          sprintf(str,"%d",num);
          name.append(ID[i]).append("read").append(str);
          ndn::Interest in(name);
          in.setScope(1);
          in.setInterestLifetime(ndn::time::milliseconds(1000));
          in.setMustBeFresh(true);
           ndn::Name alice("/ndn/test/alice1");
          // keyChain_.createIdentity(alice);
          // keyChain_.signByIdentity(i,alice);
          ndn::CommandInterestGenerator generator;
          generator.generateWithIdentity(in, alice);
          ndn::Face face;

          face.expressInterest(in,
                                ndn::bind(&Producer::get, this, boost::ref(face), _1, _2),
                                ndn::bind(&Producer::onTimeout, this, boost::ref(face), _1));
          face.processEvents();
        }
        ndn::Data data(ndn::Name(interest.getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        sprintf(result,"%f",ele[num]);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

    std::cout << ">> D: " << data << std::endl;
    face_.put(data);
      }
    }

    else if(command=="cost")
    {
      ele[1]=ele[2]=ele[3]=5;
      hasvalue[1]=hasvalue[2]=hasvalue[3]=true;
      if(hasvalue[1]==true && hasvalue[2]==true && hasvalue[3]==true)
      {
        ndn::Data data(ndn::Name(interest.getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        float cost = ele[1]*2 + ele[2]*3 + ele[3]*4;
        sprintf(result,"%f",cost);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

        std::cout << ">> D: " << data << std::endl;
        face_.put(data);
      }
      else
      {
        ele[1]=ele[2]=ele[3]=0;
        for(int i=0; i<ID_num; i++) 
        {
          for(int j=1; j<4; j++) 
          {
            num = j;
            ndn::Name name("/ndn-caculator");
            char str[15];
            sprintf(str,"%d",j);
            name.append(ID[i]).append("read").append(str);
            ndn::Interest in(name);
            in.setScope(1);
            in.setInterestLifetime(ndn::time::milliseconds(1000));
            in.setMustBeFresh(true);
             ndn::Name alice("/ndn/test/alice");
            // keyChain_.createIdentity(alice);
            // keyChain_.signByIdentity(i,alice);
            ndn::CommandInterestGenerator generator;
            generator.generateWithIdentity(in, alice);
            ndn::Face face;

            face.expressInterest(in,
                                  ndn::bind(&Producer::get, this, boost::ref(face), _1, _2),
                                  ndn::bind(&Producer::onTimeout, this, boost::ref(face), _1));
            face.processEvents();
          }
        }
        ndn::Data data(ndn::Name(interest.getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        std::cout << ">> D: " << ele[1] << std::endl;
        std::cout << ">> D: " << ele[2] << std::endl;
        std::cout << ">> D: " << ele[3] << std::endl;
        float cost = ele[1]*2 + ele[2]*3 + ele[3]*4;
        sprintf(result,"%f",cost);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

        std::cout << ">> D: " << data << std::endl;
        face_.put(data);
      }
    }

  }

  void 
  onValidated(const shared_ptr<const Interest>& interest) 
  {
    // ele[1]=12.3;
    // ele[2]=15.5;
    // ndn::Data data(ndn::Name(interest->getName()));
    // data.setFreshnessPeriod(time::seconds(10));
    // std::string command=ndn::Name(interest->getName()).get(1).toUri();
    // if(command=="plus")
    // {
    //       std::string str_1=ndn::Name(interest->getName()).get(2).toUri();
    //       std::string str_2=ndn::Name(interest->getName()).get(3).toUri();
    //       int num_1=std::atoi(str_1.c_str());
    //       char str[30];
    //       int num_2=std::atoi(str_2.c_str());
    //       float result=ele[num_1]+ele[num_2];
    //       char result_str[15];
    //       sprintf(result_str,"%f",result);
    //       std::cout <<"a" <<result_str <<"b" <<num_1 <<"c" <<num_2  << std::endl; 
    //       data.setContent((const uint8_t*)result_str, sizeof(result_str));
    // } 
    std::string command=ndn::Name(interest->getName()).get(2).toUri();
    if(command=="founded")
    {std::cout << "found" << std::endl;
      ID[ID_num]=ndn::Name(interest->getName()).get(3).toUri();
      ID_num++;
    }
    else if(command=="read")
    {

      num=std::atoi(ndn::Name(interest->getName()).get(3).toUri().c_str());
      if(hasvalue[num]==true)
      {
        ndn::Data data(ndn::Name(interest->getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        sprintf(result,"%f",ele[num]);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

        std::cout << ">> D: " << data << std::endl;
        face_.put(data);
      }
      else
      {
        for(int i=0; i<ID_num; i++) 
        {
          ndn::Name name("/ndn-caculator");
          char str[15];
          sprintf(str,"%d",num);
          name.append(ID[i]).append("read").append(str);
          ndn::Interest in(name);
          in.setScope(1);
          in.setInterestLifetime(ndn::time::milliseconds(1000));
          in.setMustBeFresh(true);
           ndn::Name alice("/ndn/test/alice1");
          // keyChain_.createIdentity(alice);
          // keyChain_.signByIdentity(i,alice);
          ndn::CommandInterestGenerator generator;
          generator.generateWithIdentity(in, alice);
          ndn::Face face;

          face.expressInterest(in,
                                ndn::bind(&Producer::get, this, boost::ref(face), _1, _2),
                                ndn::bind(&Producer::onTimeout, this, boost::ref(face), _1));
          face.processEvents();
        }
        ndn::Data data(ndn::Name(interest->getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        sprintf(result,"%f",ele[num]);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

    std::cout << ">> D: " << data << std::endl;
    face_.put(data);
      }
    }

    else if(command=="cost")
    {
      ele[1]=ele[2]=ele[3]=5;
      hasvalue[1]=hasvalue[2]=hasvalue[3]=true;
      if(hasvalue[1]==true && hasvalue[2]==true && hasvalue[3]==true)
      {
        ndn::Data data(ndn::Name(interest->getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        float cost = ele[1]*2 + ele[2]*3 + ele[3]*4;
        sprintf(result,"%f",cost);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

        std::cout << ">> D: " << data << std::endl;
        face_.put(data);
      }
      else
      {
        ele[1]=ele[2]=ele[3]=0;
        for(int i=0; i<ID_num; i++) 
        {
          for(int j=1; j<4; j++) 
          {
            num = j;
            ndn::Name name("/ndn-caculator");
            char str[15];
            sprintf(str,"%d",j);
            name.append(ID[i]).append("read").append(str);
            ndn::Interest in(name);
            in.setScope(1);
            in.setInterestLifetime(ndn::time::milliseconds(1000));
            in.setMustBeFresh(true);
             ndn::Name alice("/ndn/test/alice");
            // keyChain_.createIdentity(alice);
            // keyChain_.signByIdentity(i,alice);
            ndn::CommandInterestGenerator generator;
            generator.generateWithIdentity(in, alice);
            ndn::Face face;

            face.expressInterest(in,
                                  ndn::bind(&Producer::get, this, boost::ref(face), _1, _2),
                                  ndn::bind(&Producer::onTimeout, this, boost::ref(face), _1));
            face.processEvents();
          }
        }
        ndn::Data data(ndn::Name(interest->getName()));
        data.setFreshnessPeriod(time::seconds(10));
        char result[15];
        std::cout << ">> D: " << ele[1] << std::endl;
        std::cout << ">> D: " << ele[2] << std::endl;
        std::cout << ">> D: " << ele[3] << std::endl;
        float cost = ele[1]*2 + ele[2]*3 + ele[3]*4;
        sprintf(result,"%f",cost);
        data.setContent((const uint8_t*)result, sizeof(result));
        keyChain_.sign(data);

        std::cout << ">> D: " << data << std::endl;
        face_.put(data);
      }
    }

    // keyChain_.sign(data);

    // std::cout << ">> D: " << data << std::endl;
    // face_.put(data);
  }

  void onValidationFailed(const shared_ptr<const Interest>& interest, const std::string& failInfo) 
  {
    std::cerr << "ERROR:Validation "<< std::endl;
    //face_.shutdown ();
  }

  void
  onRegisterFailed (const ndn::Name& prefix, const std::string& reason)
  {
    std::cerr << "ERROR: Failed to register prefix in local hub's daemon ("<< reason <<")"  << std::endl;
    face_.shutdown ();
  }
  
  void
  listen()
  {
    face_.setInterestFilter("/ndn-caculator/ID1",
                            func_lib::bind(&Producer::onInterest, this, _1, _2),
                            func_lib::bind(&Producer::onRegisterFailed, this, _1, _2));
    face_.setInterestFilter("/ndn-caculator/ID",
                            func_lib::bind(&Producer::onInterest, this, _1, _2),
                            func_lib::bind(&Producer::onRegisterFailed, this, _1, _2));

    face_.processEvents();
  }

  void
  find_ID()
  {
    std::cout << "finding "<< std::endl;
    ndn::Interest i(ndn::Name("/ndn-caculator/ID1_find"));
    i.setScope(1);
    i.setInterestLifetime(ndn::time::milliseconds(1000));
    i.setMustBeFresh(true);
     ndn::Name alice("/ndn/test/alice");
    // keyChain_.createIdentity(alice);
    // keyChain_.signByIdentity(i,alice);
    ndn::CommandInterestGenerator generator;
    generator.generateWithIdentity(i, alice);
    ndn::Face face;
    face.expressInterest(i,
                          ndn::bind(&Producer::onData, this, boost::ref(face), _1, _2),
                          ndn::bind(&Producer::onTimeout, this, boost::ref(face), _1));
    face.processEvents();
  }

  void
  onData(ndn::Face &face,
       const ndn::Interest& interest, ndn::Data& data){}

  void
  onTimeout(ndn::Face &face,
          const ndn::Interest& interest){}

  void
  get(ndn::Face &face,
         const ndn::Interest& interest, ndn::Data& data)
  {
    std::string str=(const char*)data.getContent().value();
    float str_num=std::atof(str.c_str());
    ele[num]+=str_num;
    hasvalue[num]=true;
  }

private:
  ndn::Face face_;
  KeyChain keyChain_;
  CommandInterestValidator validator_;
  Buffer ndndId_;
  float ele[10];
  bool hasvalue[10];
  std::string ID[10];
  int ID_num;
  int num;
};

int main()
{

  try {
    Producer producer;
    producer.find_ID();
    producer.listen();
  }
  catch(std::exception &e) {
    std::cerr << "ERROR: wrong" << e.what() << std::endl;
  }
  return 0;
}
