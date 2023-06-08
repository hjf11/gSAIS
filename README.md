Izgradnja gSAIS poboljšanja algoritma SAIS, koje omogućuje izgradnju sufiksnog polja nad kolekcijom nizova.

Prvo je potrebno prevesti gSAIS.cpp naredbom "g++ gSAIS.cpp -o gSAIS".
Prilikom pokretanja kao argumenti se zadaju imena datoteka s nizovima nad kojima želimo izgraditi sufiksno polje
(zastavica -seq) i imena datoteka s podnizovima kojima želimo pretraživati kolekciju nizova (zastavica -s).
Podnizovi za pretraživanje u datoteci bi trebali biti odvojeni znakom zareza.

Program slice_ecoli.py koristi se za dobivanje nasumičnih podnizova određene duljine genoma E. Coli.

Skripta run.sh prevodi gSAIS.cpp i pokreće ga s podacima iz mape ecoli.
