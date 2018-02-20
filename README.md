# vidilabSmarthouse


Cijeli Arduino kod možete pronaći pod nazivom vidilabSmartHouse.ino



# Shema spajanja:
![pametnakucashema1](https://user-images.githubusercontent.com/28891269/36415591-bff5b69a-1626-11e8-8c54-bf4e9b02f9e5.png)


	Digitalni pinovi 0 i 1 služe za komunikaciju arduina s bluetooth modulom, prilikom snimanja programa na arduino oni moraju biti slobodni. Rx i Tx označavaju smjer komunikaciju i zato moraju biti suprotno spojeni ako gledamo pinove na arduinu i bluetooth modulu. Rx se spaja na Tx, a Tx na Rx. Digitalne pinove 2,3,4 i 5 smo koristili kao izlaz/output za impuls uključivanja/isključivanja releja. Mogli smo ih spojiti i na analogne pinove, ali njih je dosta manje pa smo ih ostavili kako bi u budućnosti mogli dodati neke analogne senzore koji očitavaju vrijednosti. Digitalne pinove 6 i 7 smo koristili kao ulaz/input za očitavanje vrijednosti infracrvenog/IR senzora i foto senzora. Pomoću komparatora napona smo foto senzor spojili kako bi nam javio ima li svjetla ili nema. On isto tako može biti spojen na analogni ulaz i slati vrijednosti u rasponu 0-1023. Senzor tlaka i temperature zraka smo spojili na analogne ulaze jer nam šalje analogne vrijednosti. Ne smijemo zaboraviti kako vanjske komponente kao što su ploča s relejima i senzori trebaju koristiti dodatno vanjsko napajanje napona 5V jer nam arduino ne može pružiti dovoljnu količinu struje kako bi ih napajali. Kao mozak projekta ne moramo koristiti Arduino Uno kao što je prikazano na slici, to može biti bilo koja verzija. Mi smo u našem projektu koristili Croduino, a provjereno radi i na Nano te Mega verziji.
Infracrveni i foto senzori imaju potenciometre koji s komparatorom napona nudi finu regulaciju. Kod infracrvenog senzora možemo određivati udaljenost na kojoj će se aktivirati, dok kod foto određujemo intenzitet svjetlosti aktiviranja. Ovisno o regulaciji senzori će slati 0/Low ili 1/High.

# Programiranje Arduina:

1.	Uključivanje/isključivanje rasvjete, tj. releja.
```
  case 52:
  digitalWrite(Relej4,!digitalRead(Relej4));
  delay(10);
  break;
```
Ako aplikacijom pošaljemo 52, što je po ASCII broj 4. Arduino će negirati stanje izlaza Relej4 što odgovara digitalnom izlazu 5. Vremenska odgoda od 10milisekundi se dodaje zbog stabilizacije programa i naredbom break završavamo dio case petlje.
Želimo li dodati još releja za rasvjetu ili neke druge električne uređaje, postupak je isti. Definiramo ime digitalnog izlaza kako bi se lakše snašli u programu. 
const int Relej10 = 11;
Nakon toga u petlji void setup digitalni pin moramo definirati kao izlaz/output.
pinMode(Relej10,OUTPUT);
digitalni izlaz 11 ćemo u nastavku programa moći nazivati i Relej10. Unutar Case naredbe dodajemo programski blok koji je identičan gore navedenom samo moramo staviti neki drugi broj za Case petlju, na primjer Case 57 što odgovara broju 9 te kod digitalWrite naredbe uključujemo/isključujemo Relej10. Što znači da bi dio programskog koda za dodatni relej10 izgledao ovako.
```
   case 57:						                            //Ako primi broj 9 
   digitalWrite(Relej10,!digitalRead(Relej10));	  //Negiraj vrijednost na izlazu Relej10
   delay(10);				                              //Pričekaj 10milisekundi
   break;					                                //Kraj dijela Case petlje
```

# Zašto ASCII? Zato što bluetooth modul prenosi znamenke u obliku ASCII zapisa.

2.	Jednostavniji senzori poput Infracrvenog ili Foto.

```
int StanjeIR = digitalRead(IR);
 		Serial.println(",Vrata = " + String(StanjeIR) + ",");
  		delay(10);
```

Definiramo varijablu StanjeIR kao integer, tj. cjeloviti broj jer će nam služiti za očitavanje stanja digitalnog ulaza. To smo mogli definirati i na početku programa. DigitalRead naredbom očitavamo stanje ulaza IR, kojega smo na početku programa definirali kao ime pina 6 pomoću naredbe:

```
const int IR=6;
```

Nakon toga ispisujemo očitanu vrijednost koja može biti 1 ili 0 pomoću naredbe Serial.println. Koristimo naredbu String kako bi očitanu vrijednost pretvorili u tip String zbog ispisa. Također opet vremenska odgoda od 10milisekundi za stabilnost programa.
Želimo li dodati Foto senzor sve ćemo ponoviti isto, samo što ćemo umjesto StanjeIR definirati varijablu StanjeFOTO, te umjesto ulaza IR koristiti ćemo ulaz FOTO, broj digitalnog pina je proizvoljan, ali zbog snalaženja je najpraktičnije prilikom svakog dodavanja koristiti prvi sljedeći pin kako bi u svakom trenutku znali koliko ih je preostalo i kako bi prilikom spajanja bili uredni, pregledni i organizirani. Ako imate malo kompleksnije projekte vrlo se lako možete izgubiti u moru nepreglednih žica.

Za Foto senzor to bi izgledalo ovako:

```
	 int StanjeFOTO = digitalRead(FOTO);
  	 Serial.println("Svjetlo = " + String(StanjeFOTO) + ",");
    	delay(10);
```

3.	Ispis senzora tlaka i temperature zraka.

```
  	 tlakZraka = ocitajTlak();
  	 Serial.println("Tlak zraka = " + String(tlakZraka) + "hPa,");
 	 TempZraka = ocitajTemperaturu();
  	 Serial.println("Temp zraka = " + String(tempZraka) + "C,");
 delay(300);
 Serial.println(); 
 ```
 
Varijable tlakZraka i TempZraka su prethodno definirane u programskom bloku za izračunavanje temperature i tlaka zraka koji će se biti objašnjen u nastavku. Tipa su Double, realni brojevi veće preciznosti za razliku od Float tipa. ocitajTlak je funkcija koja svojim pozivom vraća vrijednost tlaka zraka i pridodaje ih varijabli imena tlakZraka. Vrijednost se nakon toga ispisuje s popratnim tekstom te mjernom jedinicom što smo objasnili u prethodnim primjerima.

Temperatura Zraka se odvija na isti način. Varijabli TempZraka tipa dobule se pridodaje vrijednost temperature zraka koju vraća funkcija ocitajTemperaturu. Zatim se ispisuje s popratnim tekstom te mjernom jedinicom.

Vremenska odgoda od 300milisekundi se dodaje zbog vremenskog intervala senzora, neki senzori popit DHT11 su poprilično sporiji i kod njih to vrijeme pristupa senzoru i slanje podataka ne može iznositi ispod 1000milisekundi što je 1sekunda. Morate biti svjesni da se taj vremenski delay odražava na cijeli program te će veće vrijednosti biti poprilično osjetne pri korištenju mobilne aplikacije. 

# Računanje vrijednosti tlaka i temperature zraka pomoću funkcija ocitajTlak i ocitajTemperaturu

Funkcije su posebni mini programčići napravljeni tako da ne moramo svaki puta kada želimo izračunati neke vrijednosti pisati cijeli kod nego ih samo pozovemo na način da napišemo njihovo ime. 

Jednostavniji primjer: Ako ćemo više puta računati površinu pravokutnika u programu, ne moramo stalno pisati P = a*b….. nego ćemo napraviti funkciju double Povrsina(int a, int b) i svaki puta kada trebamo površinu samo ju pozovemo Povrsina(a,b) i unutar zagrada upišemo vrijednosti stranica pravokutnika a i b. Funkcija će negdje dalje biti definirana što radi, a u ovom slučaju će vraćati vrijednost a*b. Možda se čini kako je ovo nepotrebno, ali ako imate neke složenije matematičke operacije vidjeli bi koliko je to zapravo jednostavnije i preglednije. O optimizaciji i složenosti programa nećemo sada govoriti, ali u cilju je uvijek složenost smanjiti što je više moguće.

Osim tlaka i temperature, ovaj senzor pomoću Tlak i tlak0 može računati i nadmorsku visinu. U našem programu to nismo koristili, ali vam ostaje na raspolaganje ako vam bude trebalo.

```
double ocitajTlak()
{ char status;
  double temp, Tlak, tlak0, nadVisina;
  status = tlak.startTemperature();
  if(status != 0)
  {   delay(status);
    status = tlak.getTemperature(temp);
    if(status != 0)
    { status = tlak.startPressure(3);
      if(status != 0)
      {  delay(status);
        status = tlak.getPressure(Tlak,temp);
        if(status != 0)
        {return(Tlak);      }}}}}

double ocitajTemperaturu()
{  char status;
  double temp;
  status = tlak.startTemperature();
  if(status != 0)
  {delay(status);
    status = tlak.getTemperature(temp);
    if(status != 0)
    { return(temp);  }}
```
    
    

Napomena: Kod slanja podataka svaku vrijednost smo odvojili zarezom kako bi mobilna aplikacija znala odvojiti vrijednosti i svaku ispisati posebno u svoj Label. Prije i poslije slanja te četiri vrijednosti senzora poslali smo praznine koje ne ispisujemo u aplikaciji, a služe kako bi izbjegli ispisivanje ASCII vrijednosti kada palimo svjetlo te da izbjegnemo moguća preklapanja u poslanim podatcima. Pa zapis izgleda ovako
,Vrata = 1, Svjetlo =1, Tlak zraka = 1013hPa, Temp zraka = 21C, 
Znači imamo šest odvojenih podataka, prvi i zadnji su praznine koje NE ISPISUJEMO.  



![pametnakucashema3](https://user-images.githubusercontent.com/28891269/36415893-c46b7308-1627-11e8-8109-0e95595fd8d6.png)

# Izrada Aplikacije:
	http://ai2.appinventor.mit.edu/ Na navedenoj adresi možete pronaći online program za izradu mobilne aplikacije za android uređaje. Sastoji se od dva dijela, Vizualnog editora zvanog Designer i programskog koda u obliku blokovske sheme zvanog Blocks. 
Prvo je potrebno vizualno organizirati izgled aplikacije, dodati tipke i Labele, a zatim napisati kod jer ćete morati odabirati točno određeni gumb i Label.
Prvim spajanjem je mobilni uređaj potrebno upariti s BT modulom, za to će biti potrebna šifra. HC-05 modul nosi tvorničku šifru 1234. Poslije ćemo pokazati kako ju možete promijeniti.
Kod Bluetooth komunikacije potrebno je svaki puta aplikaciju spojiti s uređajem, tj. BT modulom. Zbog toga ćemo dodati izbornik naziva Odaberi uređaj čijim ćemo otvaranjem dobiti popis dostupnih Bluetooth uređaja. Nakon uspješnog spajanja umjesto naziva izbornika će se pojaviti ime BT modula koje se može mijenjati po želji jer je tvorničko ime modula njegova mac adresa. 
Program ima neke integrirane funkcije koje su nevidljive pa ih je potrebno odabrati kod Designer screena, a to su Clock1, TinyDB1, itd. U Clock1 je potrebno staviti TimeInterval koji će osvježavati aplikaciju, najviše se osjeti na Labelima gdje će se ispisivati vrijednosti senzora koje arduino šalje. Vrlo je bitno pogoditi taj interval osvježavanja s vremenskim delay-om arduino koda kako bi se osvježavalo u intervalima kako arduino šalje podatke. 250ms je pristojna vrijednost.

![dizajn](https://user-images.githubusercontent.com/28891269/36415746-414959f4-1627-11e8-971a-cec3cab15c89.png)
Naš vizualni dizajn izgleda ovako. S lijeve strane imate već ponuđene komponente koje ćete koristiti. Button, Label, ListPicker. Svaku komponentu možete personalizirati, tj. staviti određeno ime, oblik, boju, sliku, pozadinu.

![labelispis](https://user-images.githubusercontent.com/28891269/36415796-73eb8af8-1627-11e8-9f07-56127e12cc0b.png)
Na slici je prikazan dio blokovskog koda u Clock1 koji razdvaja šest primljenih podataka, objasnili smo ih kao vrijednosti senzora, te prvi i zadnji podatak koji su praznine. U crvenom bloku „split text at“ smo odabrali zarez jer ga rijetko koristimo i ne možemo pomiješati s nekim drugim znakovima. Svaki podatak ispisuje u odgovarajući Label od 1-6, dok su Label5 i Label6 postavljeni kao nevidljivi jer se u njima nalaze „praznine“.

![paljenjesvjetala](https://user-images.githubusercontent.com/28891269/36415756-4f5f0a7a-1627-11e8-8b37-3ef9390eafeb.png)
Za uključivanje/isključivanje rasvjetnih tijela koje smo riješili Case naredbom i ASCII znakovima koristiti ćemo „gumbe“ čijim će se pritiskom poslati određena vrijednost pod uvjetom da je BT modul spojen s aplikacijom. Kao tekst ispisujemo znakovnu vrijednost jer bluetooth sam pretvara u ASCII.

![aboutscreen](https://user-images.githubusercontent.com/28891269/36415821-878c5cd6-1627-11e8-8b9e-c29568d7bac5.png)
Naravno, ovdje možemo riješiti i detalje aplikacije pod koje spada ikonica, ime aplikacije, podaci o osobi koja ju je izradila, itd. 
Ovo je pojednostavljena verzija s osnovnim komponentama, naravno, ovdje možete dodati još i ispitivanje stanja rasvjetnih tijela koje će vam se prikazivati na aplikaciji pomoću Label-a. 
Aplikaciju možete spremiti u formatu .aia ili ju možete eksportirati u .apk oblik za instalaciju na Android uređaju. Izbornik Built/App-save .apk to my computer.

# ***	Kako preimenovati HC-05 Bluetooth modul?
![hc05](https://user-images.githubusercontent.com/28891269/36415856-9e3869f2-1627-11e8-87cc-c17df62c594b.jpg)

Kod koji je potrebno snimiti na Arduino:

```arduino
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX i TX pinovi
void setup()
{
pinMode(9, OUTPUT); // Key pin stavi u HIGH kako bi otvorio AT mod
digitalWrite(9, HIGH);
Serial.begin(9600);
Serial.println("Unesi AT naredbu= ");
BTSerial.begin(38400); // HC-05 brzina u AT modu
}

void loop()
{
if (BTSerial.available())
Serial.write(BTSerial.read());
if (Serial.available())
BTSerial.write(Serial.read());
```

 
1.	HC-05 spojiti kako je prikazano na slici.
2.	Prije nego li spojite Arduino s računalom, otpojite napajanje bluetooth modula. VCC pin, tj. 5V
3.	Spojite Arduino s računalom
4.	Provjerite da BT modul nije spojen/uparen trenutno s nijednim uređajem
5.	Spojite napajanje na BT modul
6.	LEDica na BT modulu bi trebala treperiti u intervalima od 2 sekunde što nam govori kako je u AT modu
7.	Kako bi provjerili je li sve uredu otvorite Serial Monitor u Arduino programu i upišite „AT“, kao povratnu informaciju trebali bi dobiti poruku „OK“
8.	Nakon toga možete mijenjati postavke BT modula, kao što je brzina prijenosa, Ime, Šifra, itd. Kako bi promijenili ime upišite naredbu „AT+NAME=IME“, Kako bi promijenili šifru u 0000 upišite naredbu „AT+PSWD=0000“
Popis AT naredbi možete pronaći na internetu.
