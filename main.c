#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
typedef struct {
    int id;
    float x;
    float y;
    float z;
}Drone;
float calcule_distance(Drone *d2, Drone *d1){
    float dx=d2->x - d1->x;
    float dy=d2->y - d1->y;
    float dz=d2->z - d1->z;
    float distance= sqrt(dx*dx+dy*dy+dz*dz);
    return distance;
}
bool respect_distance(Drone *d1,Drone *d2, int dmin ){
    float distance= calcule_distance(d1,d2);
    if (distance>=dmin){
        return true;
    }else{
        return false;
    }
}
void corriger_distance(Drone *a, Drone *b, int dmin){
    float distance=calcule_distance(a,b);
    if (!respect_distance(a,b,dmin)&& distance>0){
        float correcteur=dmin/distance;
        b->x= a->x+(b->x - a->x)*correcteur;
        b->y= a->y+(b->y - a->y)*correcteur;
        b->z= a->z+(b->z - a->z)*correcteur;
    }
}
void echange(Drone *a,Drone *b){
    Drone t= *a;
    *a = *b;
    *b = t;
}
Drone *partie(Drone *gauche, Drone *droite){
    Drone *milieu=gauche+(droite - gauche)/2;
    if(milieu->x < gauche->x){
        echange(milieu , gauche);
    }
    if (milieu->x > droite->x){
        echange(milieu,droite);
    }
    if (droite->x < gauche->x){
    echange(droite , gauche);
    }
    echange(milieu, gauche);
    float pivot = gauche->x;
    Drone *i= gauche;
    for (Drone *j = gauche+1; j <= droite; j++){
        if(j->x < pivot){
            i++;
            echange(i,j);
        }
    }
    echange (gauche,i);
    return i;
}
void quicksort(Drone *left, Drone *right){
    if (left<right){
        Drone *p=partie(left,right);
        if (p>left){
                quicksort(left,p-1);
        }
        quicksort(p+1,right);
    }
}
int main(){
    int N=10000;
    Drone *drone=malloc(N*sizeof(Drone));
    if (drone==NULL){
        return 1;
    }
    for (int k=0; k<N;k++){
        Drone *actuel=drone+k;
        actuel->id = k;
        if (k==1){
            actuel->x = 2.0f;
            actuel->y = 0.0f;
            actuel->z = 0.0f;
        }else{
            actuel->x = (float)((k % 25) * 5);
            actuel->y = (float)(((k / 25) % 25) * 5);
            actuel->z = (float)((k / 625) * 5);
        }
    }
    quicksort(drone, drone+N-1);
    for (int i=0; i<N;i++){
        for(int j=i+1; j<N;j++){
            Drone *d1 = drone + i;
            Drone *d2 = drone + j;
            if((d2->x - d1->x)>5) break;
            if (!respect_distance(d1,d2,5)){
                printf("alerte: Drones %d et %d trop proches!\n", d1->id, d2->id);
                corriger_distance(d1,d2, 5);
            }
        }
    }
    free(drone);
    return 0;
}
