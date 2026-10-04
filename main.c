#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char name_surname[20];
    char title[20];
    char id[5];

}voluntary;
typedef struct {
    char id[5];
    int amount;
}donor;
typedef struct {
    char id[5];
    int amount;
    int numb;
}blood;

voluntary *read_voluntary_file(int *count,char *filename) {
    FILE *fp1=fopen(filename,"r");
    if (fp1==NULL) {
        perror("error reading file");
        *count=0;
        exit(1);


    }
    int capacity=100;
    int n=0;
    voluntary *arr=malloc(capacity*sizeof(voluntary));
    if (arr==NULL){
        perror("memory allocation failed");
        fclose(fp1);
        *count=0;
        exit(1);

    }
    while (fscanf(fp1,"%s %s %s ",arr[n].name_surname,arr[n].title,arr[n].id)==3) {
        n++;
        if (n==capacity) {
            capacity*=2;
            voluntary *tmp=realloc(arr,capacity*sizeof(voluntary));
            if (tmp==NULL) {
                perror("reallocation failed");
                fclose(fp1);
                return arr;
            }

            arr=tmp;
        }

    }
    *count=n;
    fclose(fp1);
    return arr;

}

donor *read_donor_file(int *count,char *filename) {
    FILE *fp=fopen(filename,"r");
    if (fp==NULL) {
        *count=0;
        perror("reading file");
        exit(1);
    }
    int capacity=100;
    int n=0;
    donor *arr1=malloc(capacity*sizeof(donor));
    if (arr1==NULL) {
        free(arr1);
        perror("allocation error");
        exit(1);
    }
    while (fscanf(fp,"%s %d",arr1[n].id,&(arr1[n].amount))==2) {
        n++;
        if (capacity == n) {
            capacity*=2;
            donor *tmp=realloc(arr1,capacity*sizeof(donor));
            if (tmp==NULL) {
                perror("reallocation error");
                fclose(fp);
                exit(1);

            }
            arr1=tmp;


        }
    }
    fclose(fp);
    *count=n;
    return arr1;


}
blood *create_new_donor_array(donor *oldarray,int *count ) {
    int i,j,n;
    n=*count;
    blood *arr3=calloc(n,sizeof(blood));
    if (arr3==NULL) {
        free(arr3);
        exit(1);

    }
    int unique_count=0;
    for (i=0;i<n;i++) {
        int found=0;
        for (j=0;j<unique_count;j++) {
            if (strcmp(arr3[j].id,oldarray[i].id)==0) {
                arr3[j].amount+=oldarray[i].amount;
                arr3[j].numb+=1;
                found=1;
                break;
            }
        }
        if (!found) {
            strcpy(arr3[j].id,oldarray[i].id);
            arr3[j].amount+=oldarray[i].amount;
            arr3[j].numb+=1;
            unique_count++;
        }

    }*count=unique_count;
    return arr3;
}

int main(void) {
    int dcount=0;
    donor *donors=read_donor_file( &dcount,"input2.txt");
    if (donors==NULL) {
        exit(1);
    }
    int count=0;
    voluntary *voluntary_array=read_voluntary_file(&count,"input1.txt");
    if (voluntary_array==NULL) {
        exit(1);
    }
    blood *new_array=create_new_donor_array(donors,&dcount);
    FILE *fp3=fopen("input3.txt","w");
    if (fp3==NULL) {
        perror("error opening file");
        free(donors);
        free(voluntary_array);
        free(new_array);
        exit(1);
    }
    for (int i=0;i<count;i++) {
        for (int j=0;j<count;j++) {
            if (strcmp(voluntary_array[i].id,new_array[j].id)==0) {
                fprintf(fp3,"%s %d cc %d times",voluntary_array[i].name_surname,new_array[j].amount,new_array[j].numb);
            }
        }
    }
    fclose(fp3);
    free(donors);
    free(voluntary_array);
    free(new_array);

    return 0;
}