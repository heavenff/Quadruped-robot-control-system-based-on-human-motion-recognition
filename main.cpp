#include "widget.h"

#include <QApplication>
#include <XnCppWrapper.h>
//#include"laikago_sdk/laikago_sdk.hpp"
#include<iostream>
#include<math.h>
#include <QtGui>
#include <QDebug>
#include "ckinectreader.cpp"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <unistd.h>
#include <thread>

#define cout qDebug() <<"{"<<__FILE__<<":"<<__LINE__<<"}"

//using namespace laikago;
using namespace std;
using namespace xn;

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define SAMPLE_XML_PATH "/home/heavenqian/kinect_driver/OpenNI-Bin-Dev-Linux-x64-v1.5.7.10/Samples/Config/SamplesConfig.xml"
//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
xn::Context g_Context; // 定义一个Context对象(背景)
xn::ScriptNode g_scriptNode; //定义颜色图和深度图的节点对象
xn::UserGenerator g_UserGenerator; //用户生成器

XnBool g_bNeedPose = FALSE;
XnChar g_strPose[20] = "";

#define MAX_NUM_USERS 15

//---------------------------------------------------------------------------
// upd_define
//---------------------------------------------------------------------------
/*struct AAA{
    int direction;
    float deepth;
    uint32_t crc;
};

struct BBB{
    float yaw;
    float pitch;
    float roll;
    uint32_t crc;
};
AAA a={0};
BBB b={0};
LOOP loop;
UDP udp(8017,"192.168.3.103",8018,sizeof(AAA),sizeof(BBB));
//---------------------------------------------------------------------------
// send&recieve
//---------------------------------------------------------------------------
void UDPRecv()
{
    udp.Recv();
}

void UDPSend()
{
    udp.GetState((char*)&b);
    printf("%f\n",b.yaw);

    udp.Send((char*)&a);
}*/
//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

XnBool fileExists(const char *fn)
{
    XnBool exists;
    xnOSDoesFileExist(fn, &exists);
    return exists;
}

#define SAMPLE_XML_PATH_LOCAL "SamplesConfig.xml"

// Callback: New user was detected
void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d New User %d\n", epochTime, nId);
    // New user found
    if (g_bNeedPose)
    {
        g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
    }
    else
    {
        g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
}
// Callback: An existing user was lost
void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& /*generator*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Lost user %d\n", epochTime, nId);
}
// Callback: Detected a pose
void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& /*capability*/, const XnChar* strPose, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    g_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}
// Callback: Started calibration（标定，校准）
void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& /*capability*/, XnUserID nId, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Calibration started for user %d\n", epochTime, nId);
}

//校准完毕
void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& /*capability*/, XnUserID nId, XnCalibrationStatus eStatus, void* /*pCookie*/)
{
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
    {
        // Calibration succeeded
        printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);//开始跟踪用户
        g_UserGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        // Calibration failed
        printf("%d Calibration failed for user %d\n", epochTime, nId);
        if(eStatus==XN_CALIBRATION_STATUS_MANUAL_ABORT)
        {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
        }
        if (g_bNeedPose)
        {
            g_UserGenerator.GetPoseDetectionCap().StartPoseDetection(g_strPose, nId);
        }
        else
        {
            g_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }
}


#define CHECK_RC(nRetVal, what)					    \
    if (nRetVal != XN_STATUS_OK)				    \
{								    \
    printf("%s failed: %s\n", what, xnGetStatusString(nRetVal));    \
    return nRetVal;						    \
}


//    void showDepthPic()
//    {

//    }


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

//********显示摄像头数据部分
    COpenNI openni;
    if(!openni.Initial())//初始化返回1表示初始化成功
//        return 1;
        return a.exec();

    QGraphicsScene scene;
    QGraphicsView view;
    view.setScene(&scene);
    view.resize(650, 540);
    view.show();

    CKinectReader kinect_reader(openni, scene);
    kinect_reader.Start();//启动，读取数据
    qDebug() << kinect_reader.test;
//*************************************************

//    thread Mythread(showDepthPic);
//    Mythread.detach();

    //分配空间，指定父对象
    QUdpSocket *udpSocket = new QUdpSocket;

    //绑定
    static quint16 _port = 8017;
    udpSocket->bind(_port,QUdpSocket::DontShareAddress);

    XnStatus nRetVal = XN_STATUS_OK;
        xn::EnumerationErrors errors;
        const char *fn = NULL;  //fn是文件描述符，指向那个config.xml
        if    (fileExists(SAMPLE_XML_PATH)) fn = SAMPLE_XML_PATH;
        else if (fileExists(SAMPLE_XML_PATH_LOCAL)) fn = SAMPLE_XML_PATH_LOCAL;
        else {
            printf("Could not find '%s' nor '%s'. Aborting.\n" , SAMPLE_XML_PATH, SAMPLE_XML_PATH_LOCAL);
            //return XN_STATUS_ERROR;
            return a.exec();
        }
        printf("Reading config from: '%s'\n", fn);

        nRetVal = g_Context.InitFromXmlFile(fn, g_scriptNode, &errors);//这里应该是从文件里面初始化背景和节点数据
        if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
        {
            XnChar strError[1024];
            errors.ToString(strError, 1024);
            printf("%s\n", strError);
            //return (nRetVal);
            return a.exec();
        }
        else if (nRetVal != XN_STATUS_OK)
        {
            printf("Open failed: %s\n", xnGetStatusString(nRetVal));
            //return (nRetVal);
            return a.exec();
        }

        nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_USER, g_UserGenerator);
        if (nRetVal != XN_STATUS_OK)
        {
            nRetVal = g_UserGenerator.Create(g_Context);//这里应该是给用户创建一个背景，并把节点传给它
            CHECK_RC(nRetVal, "Find user generator");
        }

        XnCallbackHandle hUserCallbacks, hCalibrationStart, hCalibrationComplete, hPoseDetected;//枚举
        if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
        {
            printf("Supplied user generator doesn't support skeleton\n");
            //return 1;
            return a.exec();
        }
        nRetVal = g_UserGenerator.RegisterUserCallbacks(User_NewUser, User_LostUser, NULL, hUserCallbacks);
        CHECK_RC(nRetVal, "Register to user callbacks");
        nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationStart(UserCalibration_CalibrationStart, NULL, hCalibrationStart);
        CHECK_RC(nRetVal, "Register to calibration start");
        nRetVal = g_UserGenerator.GetSkeletonCap().RegisterToCalibrationComplete(UserCalibration_CalibrationComplete, NULL, hCalibrationComplete);
        CHECK_RC(nRetVal, "Register to calibration complete");
        //这里是在main函数里面调用外面的那几个初始化操作

        if (g_UserGenerator.GetSkeletonCap().NeedPoseForCalibration())
        {
            g_bNeedPose = TRUE;
            if (!g_UserGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
            {
                printf("Pose required, but not supported\n");
                //return 1;
                return a.exec();
            }
            nRetVal = g_UserGenerator.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, hPoseDetected);
            CHECK_RC(nRetVal, "Register to Pose Detected");
            g_UserGenerator.GetSkeletonCap().GetCalibrationPose(g_strPose);
        }

        g_UserGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

        nRetVal = g_Context.StartGeneratingAll();//开始生成背景
        CHECK_RC(nRetVal, "StartGenerating");

        XnUserID aUsers[MAX_NUM_USERS];
        XnUInt16 nUsers;
        XnSkeletonJointTransformation neck; //骨骼关节变换
        XnSkeletonJointTransformation righthandJoint;
        XnSkeletonJointTransformation lefthandJoint;
        XnSkeletonJointTransformation head;
        XnSkeletonJointTransformation rightknee;

        printf("Starting to run\n");
        cout<<"Starting to run";
        if(g_bNeedPose)
        {
            printf("Assume calibration pose\n");
            cout<<"Assume calibration pose";
        }

        while (!xnOSWasKeyboardHit())
        {
            QApplication::processEvents();//防止进入循环时间较长后窗口变黑按钮失效

            g_Context.WaitOneUpdateAll(g_UserGenerator);
            // print the torso information for the first user already tracking
            nUsers=MAX_NUM_USERS;
            g_UserGenerator.GetUsers(aUsers, nUsers);//获取视野内的用户
            for(XnUInt16 i=0; i<nUsers; i++)
            {
                if(g_UserGenerator.GetSkeletonCap().IsTracking(aUsers[i])==FALSE)
                    continue;

               g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_NECK,neck);//获取骨骼关节
               /*    printf("user %d: neck at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                            neck.position.position.X,
                            neck.position.position.Y,
                            neck.position.position.Z);			*/
               g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_HAND,righthandJoint);
               /* 	printf("user %d: rh at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                            righthandJoint.position.position.X,
                            righthandJoint.position.position.Y,
                            righthandJoint.position.position.Z); */
               g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_LEFT_HAND,lefthandJoint);
               /*  printf("user %d: lh at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                    lefthandJoint.position.position.X,
                                    lefthandJoint.position.position.Y,
                                    lefthandJoint.position.position.Z);  */
               g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_HEAD,head);
               /*  printf("user %d: lh at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                    head.position.position.X,
                                    head.position.position.Y,
                                    head.position.position.Z);  */
               g_UserGenerator.GetSkeletonCap().GetSkeletonJoint(aUsers[i],XN_SKEL_RIGHT_KNEE,rightknee);
               /*  printf("user %d: lh at (%6.2f,%6.2f,%6.2f)\n",aUsers[i],
                                    head.position.position.X,
                                    head.position.position.Y,
                                    head.position.position.Z);  */
               // XnSkeletonJointPosition righthandJoint;
                    if(righthandJoint.position.position.X > neck.position.position.X &&
                            righthandJoint.position.position.Y > neck.position.position.Y )
                    {
//                        a.direction=1000;
                        printf("right hand rise\n");
                        cout<<"right hand rise";
                        //设置对方的IP和端口
                        QString ip = "192.168.3.20";
                        qint16 port = 8018;

                        //设置动作1发送的内容
                        QString str = "aaaa";

                        //给指定的IP发送数据
                        udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);

                        sleep(1);
                    }
                    if(lefthandJoint.position.position.X < neck.position.position.X &&
                            lefthandJoint.position.position.Y > neck.position.position.Y )
                    {
//                        a.direction=2000;
                        printf("left hand rise\n");
                        cout<<"left hand rise";
                        //设置对方的IP和端口
                        QString ip = "192.168.3.20";
                        qint16 port = 8018;

                        //设置动作2发送的内容
                        QString str = "bbbb";

                        //给指定的IP发送数据
                        udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);
                        sleep(1);
                    }
//                    if(lefthandJoint.position.position.X == righthandJoint.position.position.X &&
//                            lefthandJoint.position.position.Y == righthandJoint.position.position.Y )
//                    {
////                        a.direction=3000;
//                        printf("clap\n");
//                        cout<<"clap";
//                    }
                    if(righthandJoint.position.position.Y < rightknee.position.position.Y &&
                            righthandJoint.position.position.X > rightknee.position.position.X)
                    {
//                        a.direction=3000;
                        printf("right knee rise\n");
                        cout<<"right knee rise";
                        //设置对方的IP和端口
                        QString ip = "192.168.3.20";
                        qint16 port = 8018;

                        //设置动作4发送的内容
                        QString str = "dddd";

                        //给指定的IP发送数据
                        udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);

                        sleep(1);
                    }

                    if(lefthandJoint.position.position.X > righthandJoint.position.position.X &&
                            righthandJoint.position.position.Y < neck.position.position.Y &&
                            lefthandJoint.position.position.Y < neck.position.position.Y)
                    {
//                        a.direction=2000;
                        printf("Hands crossed\n");
                        cout<<"Hands crossed";
                        //设置对方的IP和端口
                        QString ip = "192.168.3.20";
                        qint16 port = 8018;

                        //设置动作3发送的内容
                        QString str = "cccc";

                        //给指定的IP发送数据
                        udpSocket->writeDatagram(str.toUtf8(),QHostAddress(ip),port);

                        sleep(1);
                    }
    /*
                    loop.SetUDPPeriod(10000);
                    loop.RegistFunc("UDP/Send", UDPSend);
                    loop.RegistFunc("UDP/Recv", UDPRecv);
                    loop.Start();*/
            }

        }
        g_scriptNode.Release();
        g_UserGenerator.Release();
        g_Context.Release();


    return a.exec();
}


