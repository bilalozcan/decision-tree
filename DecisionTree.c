#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct elma {
	double koordinatX;
	double koordinatY;
	int renk;
};
struct soru {
	int koordinatTipi;
	double koordinat;
};
struct kokDugum {
	struct soru enIyiSoru;
	struct icDugum * solIcDugum;
	struct icDugum * sagIcDugum;
};
struct icDugum {
	struct soru enIyiSoru;
	struct yaprakDugum * solYaprakDugum;
	struct yaprakDugum * sagYaprakDugum;
};
struct yaprakDugum {
	int elmaSayisi;
	struct elma * elmalar;
};
double EntropiHesaplama(struct elma data[], int dataSayisi){
	double sari_elma_sayisi=0, kirmizi_elma_sayisi=0, yesil_elma_sayisi=0;
	//Sol, sağ veya bütün data için entropi hesaplarken hangi renk elmadan kaç adet var bulunması
	for(int i = 0; i < dataSayisi; ++i){
		if (data[i].renk == 1)
			sari_elma_sayisi++;
		else if (data[i].renk == 2)
			kirmizi_elma_sayisi++;
		else if (data[i].renk == 3)
			yesil_elma_sayisi++;
	}
	//O data için o renk elmanın p(c)'si
	double sariOlasilik = (sari_elma_sayisi/dataSayisi);
	double kirmiziOlasilik = (kirmizi_elma_sayisi/dataSayisi);
	double yesilOlasilik = (yesil_elma_sayisi/dataSayisi);
	double genelEntropi = 0;
	
	//Eger o renk elmanın sayısı 0 ise o data elemanı değildir bu yuzden entropi hesabına katılmaz
	if(sari_elma_sayisi != 0){
		genelEntropi = genelEntropi + (sariOlasilik*log2(sariOlasilik));
	}
	if(kirmizi_elma_sayisi != 0){
		genelEntropi = genelEntropi + (kirmiziOlasilik*log2(kirmiziOlasilik));
	} 
	if(yesil_elma_sayisi != 0){
		genelEntropi = genelEntropi + (yesilOlasilik*log2(yesilOlasilik));
	}
	return (-1) * genelEntropi;
}

double IHesaplama(struct elma data[], int dataSayisi, struct soru sorular){
	printf("##################################### I HESAPLAMA ###################################################\n");
	printf("Elemanin ");
	if(sorular.koordinatTipi == 0)
		printf("x pozisyonu ");
	else if(sorular.koordinatTipi == 1)
		printf("y pozisyonu ");
	printf("%.2lf dan ", sorular.koordinat);
	printf ("kucuk mu?\n");
	int soldakisayisi = 0;
	int sagdakisayisi = 0;
	struct elma solData[19] = {0};
	struct elma sagData[19] = {0};
	double I = 0;
	double H_sag = 0; //Sağa gonderilen datalar için entropi değişkeni
	double H_sol = 0; //Sola gonderilen datalar için entropi değişkeni
	//Bütün dataları tarayıp verilen soruya göre sağa veya sola gönderen döngü
	for(int i = 0; i < dataSayisi; ++i){
		if(sorular.koordinatTipi == 0){
			//soru dizisinin işaretini kontrol eden kontrol. 0 ise < 'tür 
			if (data[i].koordinatX < sorular.koordinat){
					solData[soldakisayisi] = data[i];
					soldakisayisi++;
			}else{
				sagData[sagdakisayisi] = data[i];
				sagdakisayisi++;
			}
			
		} else if(sorular.koordinatTipi == 1){
			//soru dizisinin işaretini kontrol eden kontrol. 0 ise < 'tür 
			if (data[i].koordinatY < sorular.koordinat){
				solData[soldakisayisi] = data[i];
				soldakisayisi++;
			}else{
				sagData[sagdakisayisi] = data[i];
				sagdakisayisi++;
			}
		}
	}

	//Soldaki dataların yazdırılması
	printf("-------------------SOL DATA----------------------\n");
	for(int i = 0; i < soldakisayisi;++i){
		printf("x: %.2lf , y:%.2lf , renk: %d \n", solData[i].koordinatX, solData[i].koordinatY, solData[i].renk);	
	}
	//Sağdaki dataların yazdırılması
	printf("-------------------SAG DATA----------------------\n");
	for(int i = 0; i < sagdakisayisi;++i){
		printf("x: %.2lf , y:%.2lf , renk: %d \n", sagData[i].koordinatX, sagData[i].koordinatY, sagData[i].renk);	
	}
	
	printf("soldaki sayisi : %d, sagdaki sayisi : %d\n", soldakisayisi, sagdakisayisi);
	H_sol = ((double)soldakisayisi / dataSayisi) * (EntropiHesaplama(solData, soldakisayisi)); // Sol için entropi hesabı
	H_sag = ((double)sagdakisayisi / dataSayisi) * (EntropiHesaplama(sagData, sagdakisayisi)); // Sağ için entropi hesabı
	I = (EntropiHesaplama(data, dataSayisi)) - (H_sol + H_sag); //I değerinin hesaplanması
	printf ("I degeri: %lf\n", I);
	printf("##################################### I HESAPLAMA BITTI #############################################\n");
	
	
	return I;
}

struct soru RastgeleSoruUretici (int xMax, int yMax){
	struct soru rastgeleSoru;
	//0 ise x koordinatı, 1 ise y koordinatı 
	rastgeleSoru.koordinatTipi = rand()%2;
	//verilen xMax ve yMax değerleri arasında bir değer üretme
	if(rastgeleSoru.koordinatTipi == 0)
		rastgeleSoru.koordinat = (double)rand()/TMP_MAX * xMax;
	else if(rastgeleSoru.koordinatTipi == 1)
		rastgeleSoru.koordinat = (double)rand()/TMP_MAX * yMax;
	return rastgeleSoru;
}

void icDugumEgitimi(struct icDugum * icdugum ,struct elma data [], int dataSayisi , struct soru maximizeEdenSoru){
	int soldakisayisi = 0;
	int sagdakisayisi = 0;
	struct elma solData[dataSayisi];
	struct elma sagData[dataSayisi];
	for(int i = 0; i < dataSayisi; ++i){
		if(maximizeEdenSoru.koordinatTipi == 0){
			if (data[i].koordinatX < maximizeEdenSoru.koordinat){
				*(solData + soldakisayisi) = data[i];
				soldakisayisi++;
			}else{
				*(sagData + sagdakisayisi) = data[i];
				sagdakisayisi++;
			}
				
		}else if(maximizeEdenSoru.koordinatTipi == 1){
			if (data[i].koordinatY < maximizeEdenSoru.koordinat){
					*(solData + soldakisayisi) = data[i];
					soldakisayisi++;
				}else{
					*(sagData + sagdakisayisi) = data[i];
					sagdakisayisi++;
				}
		}
	}
	icdugum->solYaprakDugum = (struct yaprakDugum *) malloc (1 * sizeof(struct yaprakDugum));
	icdugum->sagYaprakDugum = (struct yaprakDugum *) malloc (1 * sizeof(struct yaprakDugum));
	icdugum->solYaprakDugum->elmalar = (struct elma *) malloc (soldakisayisi * sizeof(struct elma));
	icdugum->sagYaprakDugum->elmalar = (struct elma *) malloc (sagdakisayisi * sizeof(struct elma));
	
	icdugum->solYaprakDugum->elmaSayisi = soldakisayisi;
	icdugum->solYaprakDugum->elmalar = solData;
	
	icdugum->sagYaprakDugum->elmaSayisi = sagdakisayisi;
	icdugum->sagYaprakDugum->elmalar = sagData;
}

void kokDugumEgitimi(struct kokDugum * kokdugum ,struct elma data[],int dataSayisi, struct soru sorular[]) {
	
	double maxI = 0;
	struct soru maximizeEdenSoru;
	for(int i = 0; i < 4; ++i){
		double hesaplananI = IHesaplama(data, dataSayisi, sorular[i]);
		if(hesaplananI > maxI){
			maxI = hesaplananI;
			maximizeEdenSoru = sorular[i];
		}
	}
	printf("Maximize Eden Soru: koordinat: %lf, koordinatTipi: %d\n",maximizeEdenSoru.koordinat, maximizeEdenSoru.koordinatTipi);
	int soldakisayisi = 0;
	int sagdakisayisi = 0;
	struct elma  solData[dataSayisi];
	struct elma  sagData[dataSayisi];
	for(int i = 0; i < dataSayisi; ++i){
		if(maximizeEdenSoru.koordinatTipi == 0){
			if (data[i].koordinatX < maximizeEdenSoru.koordinat){
				*(solData + soldakisayisi) = data[i];
				soldakisayisi++;
			}else{
				*(sagData + sagdakisayisi) = data[i];
				sagdakisayisi++;
			}
		}else if(maximizeEdenSoru.koordinatTipi == 1){
			if (data[i].koordinatY < maximizeEdenSoru.koordinat){
				*(solData + soldakisayisi) = data[i];
				soldakisayisi++;
			}else{
				*(sagData + sagdakisayisi) = data[i];
				sagdakisayisi++;
			}
		}
		
	}
	time_t t;
	srand((unsigned)time(&t));
	
	
	struct soru sorularSol[4];
	for(int i = 0; i < 4; ++i){
		sorularSol[i] = RastgeleSoruUretici(5, 5);
	}
	double maxISol = 0;
	struct soru maximizeEdenSoruSol;
	for(int i = 0; i < 4; ++i){
		double hesaplananISol = IHesaplama(solData, soldakisayisi, sorularSol[i]);
		if(hesaplananISol > maxISol){
			maxISol = hesaplananISol;
			maximizeEdenSoruSol = sorularSol[i];
		}
	}
	printf("Maximize Eden SoruSOL: koordinat: %lf, koordinatTipi: %d\n",maximizeEdenSoruSol.koordinat, maximizeEdenSoruSol.koordinatTipi);
	
	struct soru sorularSag[4];
	for(int i = 0; i < 4; ++i){
		sorularSag[i] = RastgeleSoruUretici(5, 5);
	}
	double maxISag = 0;
	struct soru maximizeEdenSoruSag;
	for(int i = 0; i < 4; ++i){
		double hesaplananISag = IHesaplama(sagData, sagdakisayisi, sorularSag[i]);
		if(hesaplananISag > maxISag){
			maxISag = hesaplananISag;
			maximizeEdenSoruSag = sorularSag[i];
		}
	}
	printf("Maximize Eden SoruSag: koordinat: %lf, koordinatTipi: %d\n",maximizeEdenSoruSag.koordinat, maximizeEdenSoruSag.koordinatTipi);
	
	kokdugum->enIyiSoru = maximizeEdenSoru;
	kokdugum->sagIcDugum = (struct icDugum *) malloc (1* sizeof(struct icDugum));
	kokdugum->solIcDugum = (struct icDugum *) malloc (1* sizeof(struct icDugum));
	printf("Maximize Eden SoruKOK: koordinat: %lf, koordinatTipi: %d\n",kokdugum->enIyiSoru.koordinat, kokdugum->enIyiSoru.koordinatTipi);
	
	icDugumEgitimi(kokdugum->solIcDugum, solData, soldakisayisi, maximizeEdenSoruSol);
	icDugumEgitimi(kokdugum->sagIcDugum, sagData, sagdakisayisi, maximizeEdenSoruSag);
}



int egitimDatasiOkuma(struct elma egitimDatasi[], char dosyaAdi[]){
	FILE *f;
	f = fopen(dosyaAdi, "r");
	if(f == NULL)
		return -1;
	int elmaSayisi = 0;
	while(!feof(f)){
		fscanf(f, "%lf%lf%d", &egitimDatasi[elmaSayisi].koordinatX, &egitimDatasi[elmaSayisi].koordinatY, &egitimDatasi[elmaSayisi].renk);
		elmaSayisi++;
	}
	printf("################################ egitimDatasi.txt Dosyasindan Okunan Data ################################\n");
	for(int i = 0; i < 19; ++i){
		printf("X: %.2lf, Y: %.2lf, Renk: %d \n", egitimDatasi[i].koordinatX, egitimDatasi[i].koordinatY, egitimDatasi[i].renk);
	}
	printf("################################ Dosyadan Okunan Veriler bu kadardir. ####################################\n");
	fclose(f);
	return 1;
}



int main(int argc, char **argv)
{
	struct elma egitimDatasi[19];
	char dosyaAdi[] = "egitimDatasi.txt";
	egitimDatasiOkuma(egitimDatasi, dosyaAdi);
	time_t t;
	srand((unsigned)time(&t));
	struct soru sorular[4];
	for(int i = 0; i < 4; ++i){
		sorular[i] = RastgeleSoruUretici(5, 5);
	}
	struct kokDugum decisionTree;
	kokDugumEgitimi(&decisionTree, egitimDatasi, 19, sorular);
	printf("\n SOL IC SOL YAPRAK ELMA SAYISI: %d\n", decisionTree.solIcDugum->solYaprakDugum->elmaSayisi);
	printf("\n SOL IC SAG YAPRAK ELMA SAYISI: %d\n", decisionTree.solIcDugum->sagYaprakDugum->elmaSayisi);
	
	printf("\n SAG IC SOL YAPRAK ELMA SAYISI: %d\n", decisionTree.sagIcDugum->solYaprakDugum->elmaSayisi);
	printf("\n SAG IC SAG YAPRAK ELMA SAYISI: %d\n", decisionTree.sagIcDugum->sagYaprakDugum->elmaSayisi);
	
	return 0;
}
