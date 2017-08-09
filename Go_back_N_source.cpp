/**

   ~~~ BISMILLAH HIR RAHMANIR RAHIM ~~~

   Problem Name: Go-Back-N Sliding Window Protocol with CRC32 error detection
       Platform: Data Communication
      Algorithm: Go-Back-N , CRC32
  Author's Name: RC Tushar

----------------------------------------------------
let the light of the Shelley moon die tonight
let the sky cover herself with dark shroud, tonight
but not the hope that promised to live
Just say her, dear, don't sleep tonight
die another day, die another night
----------------------------------------------------

"Don't follow others,instead learn from their experience and make your own path"

**/



#include <bits/stdc++.h>
#include <windows.h>
#define  ps system("pause")
#define  sp(n)  Sleep(n)
#define  pf   printf
#define  pb   push_back
#define F first
#define S second
using namespace std;


string frame[50];
int n,w;

char Reminder[500]= {},Recieved[500]= {};

void crc(string input,string P)  // CRC error detection function
{
    int i,j,keylen,msglen;

    string key1(500,' ');
    char temp[500],rem[500];

    keylen=P.size();
    msglen=input.size();
    key1=P;

    for (i=0; i<keylen-1; i++)
        input[msglen+i]='0';

    for (i=0; i<keylen; i++)
        temp[i]=input[i];


    for (i=0; i<msglen; i++)
    {
        if(temp[0]=='0')
        {
            for (j=0; j<keylen; j++)
                P[j]='0';
        }
        else
        {
            for (j=0; j<keylen; j++)
                P[j]=key1[j];
        }

        for (j=keylen-1; j>0; j--)
        {
            if(temp[j]==P[j])
                rem[j-1]='0';
            else
                rem[j-1]='1';
        }
        rem[keylen-1]=input[i+keylen];
        strcpy(temp,rem);
    }

    strcpy(rem,temp);

    for (i=0; i<keylen-1; i++)
        Reminder[i]=rem[i];

    for (i=0; i<msglen; i++)
        Recieved[i]=input[i];
    for (j=0; i<msglen+keylen-1; i++,j++)
        Recieved[i]=rem[j];
}


bool ck_err(string input,string P)  // This function takes a frame and returns the state (success/damage) of the received frame
{
    int rr,gg;
    crc(input,P);     // CRC32 to find the FCS and Resultant Message

    rr=rand()%4;    // randomly picks a frame to manipulate it.
                    // If rr=0 it will manipulate the current frame

    gg=rand()%10;   // randomly picks a position from 0-9 in the
                    // current frame to flip the gg position

    if(!rr)
    {
        if(Recieved[gg]=='0')
            Recieved[gg]='1';
        else
            Recieved[gg]='0';
    }

    crc(Recieved,P); // CRC32 to check the received message's reminder

    pf("\n\n Reminder Found:  %s\n",Reminder);
    for(int i=0; Reminder[i]; i++)
        if(Reminder[i]=='1')
            return false;  // if reminder not zero returns false
    return true; // if reminder is zero returns true
}

string genar()   // generates and returns random data for a frame
{
    string Reminder;
    for(int i=0; i<51; i++)
        Reminder.pb((char)((rand()%2)+48));
    return Reminder;
}

void show()     // prints bar in middle of the frames
{
    cout<<'|';
    for(int j=0; j<n; j++)
        cout<<j<<'|';
    cout<<endl;
}

void Print_chunk(int pos,int x,int y) // Prints current window chunks which are ready to send
{
    for(int j=0; j<n; j++)
    {
        if(pos==j)
            cout<<"|";
        if(j>=x&&j<=y)
        {
            if(j<10)
                cout<<"- ";
            else
                cout<<" - ";
        }
        else
        {
            if(j<10)
                cout<<"  ";
            else
                cout<<"   ";
        }
    }
    cout<<endl<<endl;
}

void Print_discard(int x,int y) // marks & prints discarded frames
{
    for(int j=0; j<n; j++)
        if(j>=x&&j<=y)
        {
            if(j<10)
                cout<<" X";
            else
                cout<<"  X";
        }
        else
        {
            if(j<10)
                cout<<"  ";
            else
                cout<<"   ";
        }
    cout<<endl<<endl;
}

int chunk(int x,int y,string P)  // Shows all states of the Go-Back N protocol
{
    system("cls");
    pf("\n\n## Frame %d to %d is Sending\n\n",x,y);
    show();
    Print_chunk(x,x,y);
    ps;

    for(int i=x; i<=y; i++)  // Sending frames (x-y)
    {
        system("cls");
        pf("\n\nData Of Frame %d: %s\n",i,frame[i].c_str());

        pf("\n\nData Of Divisor: %s ",P.c_str());

        if(ck_err(frame[i],P))
        {
            pf("\n\n ## Frame %d received successfully\n\n\n",i);
            show();
            Print_chunk(x,i+1,y);
            ps;
        }
        else
        {

            pf("\n\nError detected in Frame %d !!! \n\n",i);
            pf("## Frame %d to %d has discard \n\n",i,y);
            show();
            Print_discard(i,y);
            pf("\n\nNegative Acknowledgment (NAK-%d) sent\n\n\n",i);

            ps;
            return i;
        }
    }

    system("cls");
    pf("Acknowledgment (ACK-%d) sent\n\n\n",y+1);
    ps;
    return y+1;
}

int main()
{
    string P="100000100110000010001110110110111";  // P value of CRC32
    srand(time(NULL));

    pf("Give Total Frame Size: ");
    cin>>n;
    pf("Give Window Size: ");
    cin>>w;

    //10 random 50 bit-frame generated
    for(int i=0; i<n; i++)
        frame[i]=genar();

    int i=0;
    while(i<n)
        i=chunk(i,min(i+w-1,n-1),P);

    return 0;
}
