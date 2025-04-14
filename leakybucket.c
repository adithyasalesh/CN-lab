#include<stdio.h>
void main()
{
    int in,out,bsize,bucket=0,n;
    printf("Enter the number of inputs:");
    scanf("%d",&n);
    printf("Enter the outgoing rate:");
    scanf("%d",&out);
    printf("Enter the bucketsize:");
    scanf("%d",&bsize);
    while(n!=0)
    {
        printf("Enter the incoming packet rate;:");
        scanf("%d",&in);
        if(in<=(bsize-bucket))
        {
            bucket=bucket+in;
            printf("bucket status=%d out of %d\n",bucket,bsize);
        }
        else
        {
          printf("dropped packets=%d\n",in-(bsize-bucket));
          bucket=bsize;
          printf("bucket status=%d out of %d\n",bucket,bsize);

        }
        bucket=bucket-out;
        printf("the outgoing bucket status %d out of %d\n",bucket,bsize);
        n--;
    }

}