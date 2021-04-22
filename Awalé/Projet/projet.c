#include <stdio.h>
#include "../lib/libgraphique.h"

typedef struct Joueurs{

	int joueur1;
	int joueur2_est_humain;
	int scorej1;
	int scorej2;
	int x;						//utilisÃ© que par l'IA

}Joueurs;

void affichage(Joueurs,int c[]);				//affiche le plateau de jeu et les graines, les scores, qui doit jouer
int adversaire_est_famine(Joueurs,int c[]);			//renvoie 1 si l'adversaire est en famine
int validation_cases(int x, Joueurs,int c[]);			//renvoie 1 si la case d'indice x dans la table des cases du plateau est peut Ãªtre jouÃ©e
Joueurs choixIA(int c[],Joueurs j);			//IA qui renvoie une simulation d'un joueur et le IA.x
int selection(Joueurs,int c[]);			//renvoie l'indice x de la case choisie si elle est valide
int distribution(int x,int c[]);			//distribue les graines de la case d'indice x dans les cases suivantes puis renvoie l'indice lx de la derniÃ¨re case modifiÃ©e
int comptage_points(int lx,Joueurs,int c[]);			//compte les point et retire les graines qui ont Ã©tÃ© comptÃ©es en partant de lx
int l_amour_toujours (int c[],Joueurs);				//renvoie 0 si la partie doit s'arrÃªter
void graines(Point contenant,int nb_graines);			//fonctions inclue dans affichage() qui affiche le nombre de graines d'une case
void GAMEOVER(int jg,Joueurs j);
int menu(void);


int main(void) {


	ouvrir_fenetre(800,600);

	Joueurs joueurs;
	joueurs.joueur1 = 1;						//Pour savoir si c'est le tour du joueur 1
	joueurs.joueur2_est_humain = menu();				//Pour savoir si le joueur 2 est un bot
	joueurs.scorej1=0;
	joueurs.scorej2=0;
	joueurs.x=0;
										
    
    int c[12] = {4,4,4,4,4,4,4,4,4,4,4,4}; //Une case mesure 116 pixels
    
	int case_selectionnee;
	int index_derniere_graine_ajoutee;
	int jeu_en_cours=1;
	int gameover;

	affichage(joueurs,c);

    while (jeu_en_cours) {
		affichage(joueurs,c);            //fonction d'affichage
		case_selectionnee = selection(joueurs, c);
		index_derniere_graine_ajoutee = distribution(case_selectionnee, c);
		
		if(joueurs.joueur1){ joueurs.scorej1 += comptage_points(index_derniere_graine_ajoutee, joueurs, c);}		//comptage des points
		else { joueurs.scorej2 += comptage_points(index_derniere_graine_ajoutee, joueurs, c);}
		
		gameover = l_amour_toujours(c,joueurs);			//regarde si la partie est terminée

		if (gameover!=1) {jeu_en_cours=0;} 
		
		int mod=joueurs.joueur1;					//tour suivant
		if (mod){joueurs.joueur1=0;}
		else {joueurs.joueur1=1;}
    }
	GAMEOVER(gameover,joueurs);					//ecran de Game Over
	attente(1000);
	
	attendre_clic();
	fermer_fenetre();
	return 0;
}




int adversaire_est_famine(Joueurs j,int c[]) {				//vÃ©rifie si les cases de l'adversaire sont vides (renvoie un 0 ou un 1)
    int i,max_c;
	
	if(j.joueur1) {max_c=12;i=6;}				//Ã©limine les cases du joueur adverse
	else {max_c=6;i=0;}
	
	while(i<max_c){						//vÃ©rifie si chaque case est vide (0)
		if (c[i]!=0) {return 0;}
		i++;
	}
	return 1;
}

int validation_cases(int x, Joueurs j,int c[]) {
	
	if (j.joueur1 && adversaire_est_famine(j,c)) {
		return (c[x]!=0 && c[x]<7-x);
	}
	else if (!j.joueur1 && adversaire_est_famine(j,c)) {
		return ( c[x]!=0 && c[x+6]<7-x);
	}
	else if (j.joueur1) {
		return (c[x]!=0);     			//renvoie Vrai si le joueur est j1, la case est une des 6 premiÃ¨res, il n'y a pas 0 graines dans la case, et si la rÃ¨gle de famine est respectÃ©e (respectivement)
	}
	else if (!j.joueur1 ) {
		return (c[x]!=0);		//renvoie Vrai si le joueur est j2, la case est une des 6 derniÃ¨res, il n'y a pas 0 graines dans la case, et si la rÃ¨gle de famine est respectÃ©e (respectivement)
	}
	return 0;
}

int selection (Joueurs j,int c[]) {				//Renvoie l'index de la case sÃ©lectionnÃ©e et boucle si c'est pas une case valide
	int x,minH,maxH;						//x est l'index de la case sÃ©lÃ©ctionnÃ©e
	do {
		if(j.joueur2_est_humain || j.joueur1) {								//Est vrai que si le mode est 2 joueurs humains
			if (j.joueur1==0){minH=300;maxH=500;}
			else {minH=100;maxH=300;}
			Point p;
			do {
			p = attendre_clic();
			}while(p.y>minH && p.y<maxH);
			x = (p.x-p.x%133)/133;
			if(j.joueur1==0) {x=11-x;}
		}
		else if (j.joueur1==0) {
			Joueurs IA = choixIA(c,j);
			x=IA.x;
		}
	}while (!validation_cases(x,j,c));				//recommence tant que la case n'est pas valide
	return x;
	
}

int distribution(int x,int c[]){                // distribue les graines et renvoie la derniÃ¨re case oÃ¹ il y a eu ajout
    int i=0,a;
	a=0;
	
    while(i<c[x]){					//s'effectue autant de fois qu'il y a de graines


        if (x+1+i==12) {a=c[x]-i;break;}			//regarde si on dépasse la 12ème case
        c[x+1+i]+=1;		//ajoute 1 graine à la case d'après
        i++;

    }
	
	if (a==0) {					//si ça dépasse pas la 12ème case
		c[x]=0;
		return x+i;
	}
	
    if (x+1+i==12) {				//si ça dépasse la deuxième case
        i=0;
        while(i<a) {

            if(i==x) {i++;a++;continue;}			//saute la case de départ
            c[i]+=1;								//ajoute 1 graine
            i++;
            if (i==12) {a-=12;i=0;}					//recommence la boucle si ça redepasse la 12ème case
        }
		
    }
	c[x]=0;			//remet à 0 la case de départ
    return i-1;		// renvoi la dernière case
}

int comptage_points(int lx,Joueurs j,int c[]) {		//compte les points
	int a,casemin,casemax;
	int pasfini;
	a=0;
	pasfini=1;
	 
	if (j.joueur1) {casemin=6;casemax=12;}				//on ne peut prendre que des graines du camp de l'adversaire
	else {casemin=0;casemax=6;}	
	
	while(pasfini){
		if ((c[lx]==2 || c[lx]==3) && lx<casemax && lx>=casemin) {						//si la case a 2 ou 3 graines, les ajoute a "a" qui est le score du tour jouÃ©
			a+= c[lx];
			c[lx]=0;										//la case est remise Ã  0
			lx--;											//on recule d'une case
			if (lx<0){lx=11;}								//si la case est plus dans le plateau, on la remet dedans
		}
		else {pasfini=0;}
	}
	return a;
	return 0;
 
}
 


Joueurs choixIA(int c[],Joueurs j) {
	
	int cIA[12],x_valides[6];			//plateau privÃ©, coups jouables, l'index de x_valides
	int i,scrIA,lx,bestscore;				//i, derniere case renvoyÃ©e par comptage_points et scores thÃ©oriques
	bestscore=0;
	Joueurs IA;
	IA.joueur1=0;
	IA.scorej2=j.scorej2;
	
	for(i=0;i<12;i++) {
		cIA[i]=c[i];
	}

	for(i=6;i<12;i++){
		if(validation_cases(i,j,c)) {
			x_valides[i]=1;								//si la case est valide, la rends Ã©ligible Ã  la selection
		}
		else {x_valides[i]=0;}
	}

	for (i=6;i<12;i++) {
		if (x_valides[i]){													//si la case est Ã©ligible, fait une simulation de la distribution des graines et du score potentiel. Si le score est meilleur que le prÃ©cedent, choisi i en tant que numero de case choisie
			lx=distribution(i,cIA);
			scrIA = comptage_points(lx,IA,cIA);
			if(bestscore<scrIA) {bestscore=scrIA; IA.x=i;}
			
		}
	}
	if (bestscore==0) {IA.x=entier_aleatoire(6)+6; }
	attente(400);
	return IA;
}

int l_amour_toujours (int c[], Joueurs j) {	//renvoie 1 si le joueur qui doit jouer est en famine ou un des scores est 25 ou plus (fin de la partie)
	int a,j1;
	a=0;
	j1=j.joueur1;
	j.joueur1=!j1;			//si le joueur est le joureur 1, j1 est Ã©gal Ã  1, et adversaire_est_famine regardera si l'adversaire du joueur qui n'est pas le joueur 1 est en famine
	
	if (j1 && adversaire_est_famine(j,c) && j.joueur1!=0){			//le joueur 2 gagne
		a=2;
	}
	if (j1 && adversaire_est_famine(j,c) && j.joueur1==0){			//le joueur 1 gagne
		a=3;
	}
	else if(!(j1 && adversaire_est_famine(j,c))){				//personne ne gagne
		a=1;
	}
	if (j.scorej1>24) {						//joueur1 gagne
		a=3;
	}						
	else if (j.scorej2>24) {				//joueur2 gagne
		a=2;
	}					
	
	return a;
}
 
 void affichage (Joueurs j,int c[])
	{
	
	
//reset de l'Ã©cran
	Point reset ={0,0};
	dessiner_rectangle(reset,800,600,black);

//formation du rectangle de jeu aux bords arrondis
	int i,p;

	Point coin1;
	coin1.x=25;
	coin1.y=100;

	Point coin2;
	coin2.x=0;
	coin2.y=125;

	dessiner_rectangle(coin1,750,400,blanc);
	dessiner_rectangle(coin2,800,350,blanc);


	Point ctr_disque;
	ctr_disque.x=24;
	ctr_disque.y=124;


	for (i=0;i<2;i++)
		{
		for (p=0;p<2;p++)
			{
			dessiner_disque(ctr_disque,25,blanc);
			ctr_disque.x=776;
			}
		ctr_disque.x=24;
		ctr_disque.y=475;
		}

//rectangle du tour des joueurs
	Point tour_j;
	tour_j.x=600;

	if (j.joueur1)
		{
		tour_j.y=550;
		dessiner_rectangle(tour_j,200,50,blanc);
		}
	else
		{
		tour_j.y=0;
		dessiner_rectangle(tour_j,200,50,blanc);
		}


//rectangle de sÃ©paration entre les contenants des deux joueurs

	Point p_mediane;
	p_mediane.x=40;
	p_mediane.y=297;

	dessiner_rectangle(p_mediane,720,6,noir);

//les six contenants du joueur 2 et les six contenants du joueur 1
//c[i]

	Point contenant;
	contenant.x=75;
	contenant.y=198;

	int mod =0;
	for (i=0;i<2;i++)
		{
		for (p=0;p<6;p++)
			{
			dessiner_disque(contenant,55,noir);
			
			if (mod){graines(contenant,c[p]);}
			else {graines(contenant,c[11-p]);}
			
			contenant.x+=130;
			}
		contenant.x=75;
		contenant.y=402;
		mod++;
		}

//disque pour les squelettes des points marquÃ©s

	Point squelette_score_tot;
	squelette_score_tot.x=25 ;
	squelette_score_tot.y=75;

	for (i=0;i<2;i++)
		{
		for(p=0;p<25;p++)
			{
			dessiner_disque(squelette_score_tot,10,blanc);
			dessiner_disque(squelette_score_tot,8,noir);
			squelette_score_tot.x+=22;
			}
		squelette_score_tot.x=25;
		squelette_score_tot.y=525;
		}


//Affiche les points marquÃ©s par les joueurs
	
	Point score_tot;
	score_tot.x=25;
	score_tot.y=75;
	int scores[2]={j.scorej2,j.scorej1};
	for (p=0;p<2;p++) {
		for (i=0;i<scores[p];i++)
			{
			dessiner_disque(score_tot,7,gris);
			score_tot.x+=22;
			}

		score_tot.x=25;
		score_tot.y=525;
	}
	






//Affichage du nombres de graines dans chaque case
	


	actualiser();

}



void graines(Point contenant,int nb_graines)
	{
	char str[2];
	contenant.x-=20;
	contenant.y-=20;
	sprintf(str,"%d",nb_graines);
	afficher_texte(str,20,contenant,blanc);
	}

int menu(void) {

	Point cmenu;
	cmenu.x=300;
	cmenu.y=40;

	afficher_texte("Menu",75,cmenu,blanc);


	Point pligne1;
	pligne1.x=40;
	pligne1.y=225;

	Point pligne2;
	pligne2.x=40;
	pligne2.y=325;

	Point pligne3;
	pligne3.x=40;
	pligne3.y=375;

	Point pligne4;
	pligne4.x=40;
	pligne4.y=475;

	dessiner_rectangle(pligne1,720,3,blanc);
	dessiner_rectangle(pligne2,720,3,blanc);
	dessiner_rectangle(pligne3,720,3,blanc);
	dessiner_rectangle(pligne4,720,3,blanc);


	Point cplayer;
	cplayer.x=263;
	cplayer.y=244;

	Point cia;
	cia.x=330;
	cia.y=394;

	afficher_texte("2 JOUEURS",50,cplayer,blanc);
	afficher_texte("VS CPU",50,cia,blanc);

	actualiser();

	Point choix;
	while(1)
		{
		choix = attendre_clic();

		if(choix.y >=225 && choix.y <= 325)
			{
			return 1;									//2 joueurs
			}
		else if(choix.y >= 375 && choix.y <= 475)
			{
			return 0;									//vs cpu
			}
		}
	return 0;

}

void GAMEOVER(int jg,Joueurs j) {

	Point p = {0,0};
	Point t={200,260};
	dessiner_rectangle(p,800,600,black);
	

	if (jg==2 && !(j.joueur2_est_humain)) {afficher_texte("LE CPU GAGNE!!",50,t,blanc);}			//le cpu gagne
	else if (jg==2) {afficher_texte("LE JOUEUR 2 GAGNE!!",50,t,blanc);t.x=120;t.y=260;}				//le joueur 2 gagne
	else if (jg==3) {afficher_texte("LE JOUEUR 1 GAGNE!!",50,t,blanc);t.x=120;t.y=260;}				//le joueur 1 gagne

}
