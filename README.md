# Implementacija učinkovitog algoritma za izgradnju poboljšanog sufiksnog polja

## Opis projekta

Ovaj projekt se bavi implementacijom algoritma za izgradnju poboljšanog sufiksnog polja (**gSAIS**) nad kolekcijom nizova. Algoritam **gSAIS** predstavlja optimiziranu verziju popularnog **SAIS** algoritma, koji omogućuje učinkovitiju izgradnju sufiksnog polja u linearnom vremenu. Program omogućuje pretragu podnizova unutar kolekcije nizova, čime se optimizira rad u bioinformatičkim aplikacijama koje se bave velikim količinama podataka, poput DNA i proteinskih sekvenci.

Projekt je realiziran kao **završni rad** pod mentorsvom **izv. prof. dr. sc. Mirjane Domazet-Lošo** na **Fakultetu elektrotehnike i računarstva** **Sveučilišta u Zagrebu**.

## Upute za korištenje

Prvo je potrebno prevesti gSAIS.cpp naredbom "g++ gSAIS.cpp -o gSAIS".
Prilikom pokretanja kao argumenti se zadaju imena datoteka s nizovima nad kojima želimo izgraditi sufiksno polje (zastavica -seq) i imena datoteka s podnizovima kojima želimo pretraživati kolekciju nizova (zastavica -s).
Podnizovi za pretraživanje u datoteci bi trebali biti odvojeni znakom zareza.

Program slice_ecoli.py koristi se za dobivanje nasumičnih podnizova određene duljine genoma E. Coli.

Skripta run.sh prevodi gSAIS.cpp i pokreće ga s podacima iz mape ecoli.


## Sažetak

U bioinformatici se često javlja potreba za pretraživanjem velikih nizova (DNA i proteinskih sljedova) različitim podnizovima. Kako bi se taj problem riješio u razumnom vremenu, potrebno je koristiti efikasne strukture indeksiranja teksta, kao što su sufiksno stablo i sufiksno polje, koje se mogu izgraditi u linearnom vremenu. Sufiksno polje samo po sebi ima neke nedostatke u odnosu na sufiksno stablo pa se zato često proširuje i nadograđuje. Algoritam gSAIS nadogradnja je popularnog SAIS algoritma za izgradnju sufiksnog polja koja omogućuje izgradnju sufiksnog polja nad kolekcijom nizova.

**Ključne riječi**: bioinformatika, sufiksno polje, SAIS, gSAIS

## Abstract

In bioinformatics there is often need to search large strings (DNA and protein sequences) with different substrings. In order to solve this problem in reasonable time, it is necessary to use efficient text indexing structures, such as suffix trees and suffix arrays, which can be built in linear time. The suffix array itself has some disadvantages compared to the suffix tree and is often expanded and upgraded. The gSAIS algorithm is an upgrade of the popular SAIS suffix array construction algorithm that allows the construction of a suffix array over a collection of strings.

**Keywords**: bioinformatics, suffix array, SAIS, gSAIS
