#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>

int p, g, e; // public key of X
int d; // private key of X
int k; //key of Y
int Y1, Y2;

int gcd(int x, int y){
    int i, gcd=1;
    for(i=1; i<=x && i<=y; i++){
        if(x%i==0 && y%i==0)
            gcd=i;
    }
    return gcd;
}

int power(int x, int y, int z){
    int t=y,q,f=x,r=1;
    while(t>1){
        q=t%2;
        if(q==1){
            r*=f;
            r=r%z;
        }
        f=f*f;
        f=f%z;
        t/=2;
    }
    f=f*r;
    f=f%z;
    return f;
}

int isprime(int x){
    int i;
    for(i=2;i<x/2;i++){
        if(x%i==0)
            return 0;
            break;
    }
    return 1;
}

int primitiveroot(int x){
    int i, a[100],n=x-1,k=0,p,chk;
    while(n%2==0){
        a[k++]=2;
        n/=2;
    }
    for(i=3;i<=sqrt(n);i+=2){
        while(n%i==0){
            a[k++]=i;
            n/=i;
        }
    }
    if(n>2)
        a[k++]=n;
    n=x-1;
    for(p=2;p<=n;p++){
        chk=0;
        for(i=0;i<k;i++){
            if(power(p,n/a[k],n)==1){
                chk=1;
                break;
            }
        }
        if(chk==0)
            return p;
    }
    return -1;
}

void publickeyX(){
    do
        p=rand()+256;
    while(isprime(p)==0);
    g=primitiveroot(p);
    while(gcd(p,g)!=1);
    d=rand()%(p - 2)+1;
    e=power(g,d,p);
}

void encrypt(char ch){
    int n=ch;
    printf("\t\t\t\t\t\t\t\t  M=%d (%c)",n,ch);
    Y1=power(g,k,p);
    Y2=n*power(e,k,p);
    printf("\n\t\t\t\t\t\t\t\t  Y1=%d, Y2=%d",Y1,Y2);
    printf("\n\t\t\t\t\t  Sent: Y1=%d, Y2=%d",Y1,Y2);
}

int invModular(int x, int n) {
    int i;
    for(int i=1;i<n;i++)
        if((x*(i%n))%n==1)
            return i;
}

void decrypt(){
    char ch;
    int x=power(Y1,d,p);
    int z=invModular(x,p);
    x=(Y2*z)%p;
    ch=x;
    printf("\nReceived: Y1= %d, Y2=%d",Y1,Y2);
    printf("\nProcess: %d*(%d^%d)^-1 mod %d = %d",Y2,Y1,d,p,x);
    printf("\nDecrypted Message: %c\n",ch);
}

int main(){
    char msg[50];
    int i,n,en,de;
    printf("\tX\t\t\t\t\tPUBLIC\t\t\tY\n");
    publickeyX();
    printf("Public Keys: p=%d, g=%d, e=%d\nPrivate Key: d=%d\n",p,g,e,d);
    printf(" \t\t\t\t\t  Sent: p=%d, g=%d, \n\t\t\t\t\t  e=%d",p,g,e);
    printf("\n\t\t\t\t\t\t\t\t  Enter Message: ");
    scanf("%s",&msg);
    n=strlen(msg);
    k=rand();
    for(i=0;i<n;i++){
        encrypt(msg[i]);
        decrypt();
    }
    return 0;
}
