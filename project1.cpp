/* author : Erin LI
*date 2022/3/24
this program is fifo algroithms*/
#include<iostream>
using namespace std;
int array;
void memory(long long size,long long size2){
     size = 1e7;
     size2= 1e13;

}
int main()
{
    int n,at[999],wt[999],tat[999],wt2[999],avwt=0,avtat=0,i,j;
    int msize[999];
    int tmsize;
    cout<<"how many process do U want :"<<endl;
    cin>>n;

   /// cout<<"\nEnter arrivel Time\n"<<endl;
   srand(time(0));
    for(i=0;i<n;i++)
   {
       at[i]=rand()%999;
       msize[i]=rand()%888;
      cout<<"ID:"<<i+1<<""<<endl;
      cout<<"arrivel : "<<at[i]<<endl;
      cout<<"memory size :"<<msize[i];
      tmsize += msize[i];
    }
    wt[0]=0;
    //waiting time
    for(i=1;i<n;i++)
    {
        wt[i]=0;
        for(j=0;j<i;j++)
            wt[i]+=at[j];
        //wt2[i]+=wt[i];
    }


    cout<<"\nID\t\tWaiting Time\tTurnaround Time";

    //calculating turnaround time
    for(i=0;i<n;i++)
    {
        tat[i]=at[i]+wt[i];
        avwt+=wt[i];
        avtat+=tat[i];
        cout<<"\n"<<i+1<<"\t\t"<<at[i]<<"\t\t"<<wt[i]<<"\t\t"<<tat[i];
    }

    avwt/=i;
    avtat/=i;
    cout<<"\n\nAverage Waiting Time:"<<avwt;
    cout<<"\nAverage Turnaround Time:"<<avtat;
    cout<<"\ntotal time :"<<avwt*n;
    cout<<"total memory taked : "<<tmsize;


    return 0;
}
