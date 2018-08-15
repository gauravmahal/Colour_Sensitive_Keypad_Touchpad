#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;
//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV)
{        
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS(imgHSV, cvScalar(103,166,52), cvScalar(129,232,156), imgThresh); 
       return imgThresh;
} 
int main()
{
    int c;
    double posx[11][9];
    double posy[11][9];
    POINT cursor;
    int x_old=0,l=0;
    int y_old=0;
    int p,q,flagx,flagy;
    int switc=0,switc1 =0;
    int shift=0;
    CvCapture* capture =0;
    capture = cvCaptureFromCAM(1);
    if(!capture)
    {
        printf("Capture failure\n");
        return -1;
    }
    IplImage* frame=0;
    cvNamedWindow("Video");      
    cvNamedWindow("Ball");
    //iterate through each frames of the video 
    int qty,a=0;
        while(true)
    {
        cin>>c;
        frame = cvQueryFrame(capture);            
        if(!frame) break;
        frame=cvCloneImage(frame); 
        cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel
        IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
        cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
        IplImage* imgThresh = GetThresholdedImage(imgHSV);
        cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
        //  cvFindContours();
        CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
        cvMoments(imgThresh, moments, 1);
        double area = cvGetCentralMoment(moments, 0, 0);
        double moment10 = cvGetSpatialMoment(moments, 1, 0);
        double moment01 = cvGetSpatialMoment(moments, 0, 1);
        double x = moment10/area;
        double y = moment01/area;
        switch(a)
            {
                case 1: posx[0][0]=x;
                        posy[0][0]=y;
                        a=2;
                        qty=0;
                        break;
                case 2: posx[10][0]=x;
                        posy[10][0]=y;
                        a=3;
                        qty=0;
                        break;    
                case 3: posx[10][8]=x;
                        posy[10][8]=y;
                        a=4;
                        qty=0;
                        break;
                case 4: posx[0][8]=x;
                        posy[0][8]=y;
                        qty=0;
                        a=5;
                        break;
                case 0: a=1;break;
            }
        cvShowImage("Ball", imgThresh);            
     cvShowImage("Video", frame);
     //Clean up used images
     cvReleaseImage(&imgHSV);
     cvReleaseImage(&imgThresh);            
     cvReleaseImage(&frame);
     //Wait 50mS
     //If 'ESC' is pressed, break the loop
     if(a==5)break;
   }
   /*cout<<posx[0][0]<<"\t";
   cout<<posx[10][0]<<"\t";
   cout<<posx[10][8]<<"\t";
   cout<<posx[0][8]<<"\t";*/
   double s1=(posx[10][0]-posx[0][0])/11;
    double s2=(posy[0][8]-posy[0][0])/9;
    //int slope2=(posx[11][11]-posx[11][0])/(posy[11][11]-posy[11][0]);
    for(int i=1;i<11;i++)
    for(int j=1;j<9;j++)
    {
        posx[i][j]=posx[i][0]+s1*i;
        posy[i][j]=posy[0][j]+s2*j;
    }
    for(int i=1;i<11;i++)
    {for(int j=1;j<9;j++)
    cout<<posx[i][j]<<"\t";
    cout<<"\n";}     
    while(true)
    {
        frame = cvQueryFrame(capture);            
        if(!frame) break;
        frame=cvCloneImage(frame); 
        cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel
        IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
        cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
        IplImage* imgThresh = GetThresholdedImage(imgHSV);
        cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
        //  cvFindContours();
        CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
        cvMoments(imgThresh, moments, 1);
        double area = cvGetCentralMoment(moments, 0, 0);
        double moment10 = cvGetSpatialMoment(moments, 1, 0);
        double moment01 = cvGetSpatialMoment(moments, 0, 1);
        double x = moment10/area;
        double y = moment01/area;
        p=x-x_old;
        q=y-y_old;
        //  cout<<x<<"   "<<y<<"\n";
        //  cout<<"aaa"<<x_old<<"p"<<p<<"\n";
        GetCursorPos(&cursor);
        // cout<<cursor.x;
        double a=cursor.x-5*p;
        double b=cursor.y+5*q;
        if(x_old<0||x<0)
        l=0;
        else l=1;
        x_old=x;
        y_old=y;
        /*if((p>1&&p<500)||p<-1)
        flagx=1;
        else flagx=0;
        if(q>1||q<-1)
        flagy=1;
        else flagy=0;*/
        // if(l==0)
        //{cout<<"chal ja be";SetCursorPos(cursor.x,cursor.y);
        //}
        //else
        if(area>=40&&switc1==1)
        {
            if(x>posx[0][0]&&x<posx[1][0]&&y>posy[0][0]&&y<posy[0][1])
            {
                keybd_event(0x30,0x0b,0, 0);//zero
                keybd_event(0x30,0x0b,KEYEVENTF_KEYUP , 0);
                Sleep(100);
            }
            if(x>posx[1][0]&&x<posx[2][0]&&y>posy[1][0]&&y<posy[1][1])
              keybd_event(0x39,0x0a,0, 0);
            if(x>posx[2][0]&&x<posx[3][0]&&y>posy[2][0]&&y<posy[2][1])
              keybd_event(0x38,0x09,0, 0);
            if(x>posx[3][0]&&x<posx[4][0]&&y>posy[3][0]&&y<posy[3][1])
              keybd_event(0x37,0x08,0, 0);
            if(x>posx[4][0]&&x<posx[5][0]&&y>posy[4][0]&&y<posy[4][1])
              keybd_event(0x36,0x07,0, 0);
            if(x>posx[5][0]&&x<posx[6][0]&&y>posy[5][0]&&y<posy[5][1])
              keybd_event(0x35,0x06,0, 0);
            if(x>posx[6][0]&&x<posx[7][0]&&y>posy[6][0]&&y<posy[6][1])
              keybd_event(0x34,0x05,0, 0);
            if(x>posx[7][0]&&x<posx[8][0]&&y>posy[7][0]&&y<posy[7][1])
              keybd_event(0x33,0x04,0, 0);
            if(x>posx[8][0]&&x<posx[9][0]&&y>posy[8][0]&&y<posy[8][1])
              keybd_event(0x32,0x03,0, 0);
            if(x>posx[9][0]&&x<posx[10][0]&&y>posy[9][0]&&y<posy[9][1])
              keybd_event(0x31,0x02,0, 0);
            if(x>posx[0][1]&&x<posx[1][1]&&y>posy[0][1]&&y<posy[0][2])
              keybd_event(0x50,0x19,0, 0);   
            if(x>posx[1][1]&&x<posx[2][1]&&y>posy[1][1]&&y<posy[1][2])
              keybd_event(0x4f,0x18,0, 0);
            if(x>posx[2][1]&&x<posx[3][1]&&y>posy[2][1]&&y<posy[2][2])
              keybd_event(0x49,0x17,0, 0);
            if(x>posx[3][1]&&x<posx[4][1]&&y>posy[3][1]&&y<posy[3][2])
              keybd_event(0x55,0x16,0, 0);
            if(x>posx[4][1]&&x<posx[5][1]&&y>posy[4][1]&&y<posy[4][2])
              keybd_event(0x59,0x15,0, 0);
            if(x>posx[5][1]&&x<posx[6][1]&&y>posy[5][1]&&y<posy[5][2])
              keybd_event(0x54,0x14,0, 0);
            if(x>posx[6][1]&&x<posx[7][1]&&y>posy[6][1]&&y<posy[6][2])
              keybd_event(0x52,0x13,0, 0);
            if(x>posx[7][1]&&x<posx[8][1]&&y>posy[7][1]&&y<posy[7][2])
              keybd_event(0x45,0x12,0, 0);
            if(x>posx[8][1]&&x<posx[9][1]&&y>posy[8][1]&&y<posy[8][2])
              keybd_event(0x57,0x11,0, 0);
            if(x>posx[9][1]&&x<posx[10][1]&&y>posy[9][1]&&y<posy[10][2])
              keybd_event(0x51,0x10,0, 0);
            if(x>posx[0][2]&&x<posx[1][2]&&y>posy[0][2]&&y<posy[0][3])
              keybd_event(0x2c,0x41,0, 0);//print scr  
            if(x>posx[1][2]&&x<posx[2][2]&&y>posy[1][2]&&y<posy[1][3])
              keybd_event(0x4c,0x26,0, 0);
            if(x>posx[2][2]&&x<posx[3][2]&&y>posy[2][2]&&y<posy[2][3])
              keybd_event(0x4b,0x25,0, 0);
            if(x>posx[3][2]&&x<posx[4][2]&&y>posy[3][2]&&y<posy[3][3])
              keybd_event(0x4a,0x24,0, 0);
            if(x>posx[4][2]&&x<posx[5][2]&&y>posy[4][2]&&y<posy[4][3])
              keybd_event(0x48,0x23,0, 0);
            if(x>posx[5][2]&&x<posx[6][2]&&y>posy[5][2]&&y<posy[5][3])
              keybd_event(0x47,0x22,0, 0);
            if(x>posx[6][2]&&x<posx[7][2]&&y>posy[6][2]&&y<posy[6][3])
              keybd_event(0x46,0x21,0, 0);
            if(x>posx[7][2]&&x<posx[8][2]&&y>posy[7][2]&&y<posy[7][3])
              keybd_event(0x44,0x20,0, 0);
            if(x>posx[8][2]&&x<posx[9][2]&&y>posy[8][2]&&y<posy[8][3])
              keybd_event(0x53,0x1f,0, 0);
            if(x>posx[9][2]&&x<posx[10][2]&&y>posy[9][2]&&y<posy[9][3])
              keybd_event(0x41,0x1e,0, 0);
            if(x>posx[0][3]&&x<posx[1][3]&&y>posy[0][3]&&y<posy[0][5])
              keybd_event(0x0d,0x1c,0, 0); //enter
            if(x>posx[1][3]&&x<posx[2][3]&&y>posy[1][3]&&y<posy[1][4])
              keybd_event(0x90,0x45,0, 0); //num lock
            if(x>posx[2][3]&&x<posx[3][3]&&y>posy[2][3]&&y<posy[2][4])
              keybd_event(0x26,0x78,0, 0);// up
            if(x>posx[3][3]&&x<posx[4][3]&&y>posy[3][3]&&y<posy[3][4])
              keybd_event(0x4d,0x32,0, 0);
            if(x>posx[4][3]&&x<posx[5][3]&&y>posy[4][3]&&y<posy[4][4])
              keybd_event(0x4e,0x31,0, 0);
            if(x>posx[5][3]&&x<posx[6][3]&&y>posy[5][3]&&y<posy[5][4])
              keybd_event(0x42,0x30,0, 0);
            if(x>posx[6][3]&&x<posx[7][3]&&y>posy[6][3]&&y<posy[6][4])
              keybd_event(0x56,0x2f,0, 0);
            if(x>posx[7][3]&&x<posx[8][3]&&y>posy[7][3]&&y<posy[7][4])
               keybd_event(0x43,0x2e,0, 0);
            if(x>posx[8][3]&&x<posx[9][3]&&y>posy[8][3]&&y<posy[8][4])
               keybd_event(0x58,0x2d,0, 0);
            if(x>posx[9][3]&&x<posx[10][3]&&y>posy[9][3]&&y<posy[9][4])
               keybd_event(0x5a,0x2c,0, 0);
            if(x>posx[1][4]&&x<posx[2][4]&&y>posy[1][4]&&y<posy[1][5])
               keybd_event(0x27,0x7d,0, 0); //right
            if(x>posx[2][4]&&x<posx[3][4]&&y>posy[2][4]&&y<posy[2][5])
               keybd_event(0x28,0x55,0, 0);// down
            if(x>posx[3][4]&&x<posx[4][4]&&y>posy[3][4]&&y<posy[3][5])
               keybd_event(0x25,0x56,0, 0);//left
            if(x>posx[4][4]&&x<posx[5][4]&&y>posy[4][4]&&y<posy[4][5])
               keybd_event(0x6e,0x34,0, 0);// .
            if(x>posx[5][4]&&x<posx[6][4]&&y>posy[5][4]&&y<posy[5][5])
               keybd_event(0x6c,0x33,0, 0);//,
            if(x>posx[6][4]&&x<posx[7][4]&&y>posy[6][4]&&y<posy[6][5])
               keybd_event(0x6f,0x35,0, 0);// /
            if(x>posx[7][4]&&x<posx[8][4]&&y>posy[7][4]&&y<posy[7][5])
               keybd_event(0x6b,0x0d,0, 0);// =
            if(x>posx[8][4]&&x<posx[9][4]&&y>posy[8][4]&&y<posy[8][5])
               keybd_event(0x6d,0x0c,0, 0);//-
            if(x>posx[9][4]&&x<posx[10][4]&&y>posy[9][4]&&y<posy[9][5])
               keybd_event(0x09,0x0f,0, 0);// tab
            if(x>posx[0][5]&&x<posx[1][5]&&y>posy[0][5]&&y<posy[0][6])
               keybd_event(0x1b,0x01,0, 0); // esc  
            if(x>posx[1][5]&&x<posx[2][5]&&y>posy[1][5]&&y<posy[1][6])
            {
                keybd_event(0x5b,0xe0,0, 0);//start   **
                keybd_event(0x5b,0xe0,KEYEVENTF_KEYUP,0);
            }    
            if(x>posx[2][5]&&x<posx[3][5]&&y>posy[2][5]&&y<posy[2][6])
               keybd_event(0x2e,0x79,0, 0);//delete
            if(x>posx[3][5]&&x<posx[4][5]&&y>posy[3][5]&&y<posy[3][6])
               keybd_event(0x08,0x0e,0, 0);//back space   
            if(x>posx[4][5]&&x<posx[6][5]&&y>posy[4][5]&&y<posy[4][6])
               keybd_event(0x20,0x39,0, 0);// spacebar
           //     *************************************recode to remove bug of continouse pressing  of Alt, Ctrl, Shift key****************
            if(shift==0)
            {
                if(x>posx[6][5]&&x<posx[7][5]&&y>posy[6][5]&&y<posy[6][6])
                {
                    keybd_event(0x12,0x38,0, 0);//alt
                    // keybd_event(0x12,0x38,KEYEVENTF_KEYUP,0);
                    shift=1;
                }    
                if(x>posx[7][5]&&x<posx[8][5]&&y>posy[7][5]&&y<posy[7][6])
                {
                    keybd_event(0x11,0x1d,0, 0);
                    shift=1;
                } //ctrl
                if(x>posx[8][5]&&x<posx[9][5]&&y>posy[8][5]&&y<posy[8][6])
                {
                    keybd_event(0x10,0x2a,0, 0);
                    shift=1;
                }// shift
            }     
            else
            {
                if(x>posx[6][5]&&x<posx[7][5]&&y>posy[6][5]&&y<posy[6][6])
                {
                    keybd_event(0x12,0x38,KEYEVENTF_KEYUP, 0);//alt
                    // keybd_event(0x12,0x38,KEYEVENTF_KEYUP,0);
                    shift=0;
                }    
                if(x>posx[7][5]&&x<posx[8][5]&&y>posy[7][5]&&y<posy[7][6])
                { 
                    keybd_event(0x11,0x1d,KEYEVENTF_KEYUP, 0);
                    shift=0;
                }//ctrl
                if(x>posx[8][5]&&x<posx[9][5]&&y>posy[8][5]&&y<posy[8][6])
                { 
                    keybd_event(0x10,0x2a,KEYEVENTF_KEYUP, 0);
                    shift=0;
                }// shift
            }     
            if(x>posx[9][5]&&x<posx[10][5]&&y>posy[9][5]&&y<posy[9][6])
            {
                keybd_event(0x14,0x3a,0, 0);//caps
                keybd_event(0x14,0x3a,KEYEVENTF_KEYUP,0);
            }    
             if(x>posx[4][7]&&x<posx[6][7]&&y>posy[4][7]&&y<posy[4][8])
            {
                switc = 0; 
                switc1=0; 
                //cout<<"fulctuation"<<endl;
            }    
           /*   if (x>576&&x<640&&y>0&&y<50)
               {  
                    keybd_event(0x41,0x1e,0, 0);
                 //   keybd_event(0x41,0x1e,KEYEVENTF_KEYUP , 0);
                    Sleep(0);}   
             if (x>512&&x<576&&y>0&&y<50)
               {  
                     keybd_event(0x42,0x30,0, 0);
                   //  keybd_event(0x42,0x30,KEYEVENTF_KEYUP , 0);
                     Sleep(100);}   
            if(x>26&&x<79&&y>47&&y<90)
              {
                     mouse_event(0x0008,0,0,0,0);
                     mouse_event(0x0010,0,0,0,0); }*/
     }        
     else
     {
         if(area>20&&switc==0)
         { 
             SetCursorPos(a,b);
             if(x>posx[0][6]&&x<posx[4][6]&&y>posy[0][6]&&y<posy[0][7])  //right click
             { 
                 mouse_event(0x0008,0,0,0,0);
                 mouse_event(0x0010,0,0,0,0); 
             }    
             if(x>posx[6][6]&&x<posx[10][6]&&y>posy[6][6]&&y<posy[6][7])  //left click
             { 
                 mouse_event(0x0002,0,0,0,0);
                 mouse_event(0x004,0,0,0,0);
             }     
             if(x>posx[4][7]&&x<posx[6][7]&&y>posy[4][7]&&y<posy[4][8]&&switc==0)
             { 
                 //cout<<"aur be!";
                 switc1 = 1; 
                 switc=1;
                 //cout<<switc1;
                 //cout<<"pta ni chala ki nhi\n";
                 // cvWaitKey(1000);
                 //Sleep(0);
             }
         }
     }                
     // if(a>1000)
     //    { keybd_event(0x41,0x1e,0, 0);
     // keybd_event(0x41,0x1e,KEYEVENTF_KEYUP , 0);}
     cvShowImage("Ball", imgThresh);            
     cvShowImage("Video", frame);
     //Clean up used images
     cvReleaseImage(&imgHSV);
     cvReleaseImage(&imgThresh);            
     cvReleaseImage(&frame);
     //Wait 50mS
     int c = cvWaitKey(200);
     //If 'ESC' is pressed, break the loop
     if((char)c==27 ) break;
   }
   cvDestroyAllWindows() ;
   cvReleaseCapture(&capture);     
   return 0;
}
