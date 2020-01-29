1. select SYSDATE as Astazi from dual;
2. select NUME, data_nastere, TRUNC(MONTHS_BETWEEN(CURRENT_DATE, data_nastere)) from Studenti;
3. select nume, prenume, TO_CHAR(data_nastere,'day') as "Zi a saptamanii" from Studenti;
4. select concat('Elevul ', concat(prenume, concat(' este in grupa ', grupa))) as Propozitie from Studenti;
5. select nume, prenume, LPAD(bursa, 10, '$') as Bursa from Studenti where bursa is not null;
6. select concat(lower(substr(trim(nume), 1, 1)), upper(substr(trim(nume), 2))) as "Nume modificat", length(nume) as "Lungime numar" from Profesori where ASCII(Nume) = 66;
7. select nume, data_nastere, add_months(data_nastere,ceil(ceil(months_between(sysdate, data_nastere)/12))*12) as Sarbatoare from Studenti;
8. select nume, prenume, to_char(data_nastere, 'Month') from Studenti where bursa is null order by extract(month from data_nastere);
9. select nume, nvl(bursa, 0), nvl(decode(bursa, 450, 'Premiul 1', 350, 'Premiul 2', 250, 'Premiul 3', null), 'Mentiune') as Premiu from Studenti;
10a. select replace(replace(replace(replace(nume, 'i', '\0'), 'a', '\1'), '\0', 'a'), '\1', 'i') as "Nume schimbat" from Studenti;
10b. select translate(nume, "aiAI", "iaIA") from Studenti;
11. select nume, prenume, floor(months_between(sysdate, data_nastere) / 12) || ' ani ' || mod(floor(months_between(sysdate, data_nastere)),12) || ' luni si ' || to_char(floor(sysdate - add_months(data_nastere, floor(months_between(sysdate, data_nastere))))) || ' zile' as Varsta, floor(add_months(data_nastere,ceil(ceil(months_between(sysdate, data_nastere)/12))*12) - sysdate) as "Zile ramase" from Studenti;
12. select nume, last_day(sysdate) + 1 as "Noua data" , nvl(decode(bursa, 450, 450*1.1 , 350, 350*1.15, 250, 250*1.2, null), 0) as "Noua bursa" from Studenti;
13. select nume, prenume, nvl(rpad(' ', bursa/50, '*'), 'Nu are bursa') as Bursa from Studenti order by 3;

1. Considerand ca in SUA varsta majoratului e 21 de ani, identificati studentii care au 21 de ani impliniti si afisati numele concatenat cu prenumele.
	SELECT concat(nume, (concat(' ', prenume))) from Studenti where months_between(sysdate, data_nastere) >= 12*21;
2. Selectati titlul, anul si numarul de credite pentru fiecare curs sub formatul 'Cursul <titlu_curs> se face in anul <an> si are un numar de <credite> credite', doar
	pentru cursurile care se fac in semestrul 1, indiferent de anul de studiu si are numele scris in majuscule
	SELECT 'Cursul ' || titlu_curs || ' se face in anul ' || an || ' si are un numar de ' || credite || ' credite' as Cursuri 
		from Cursuri 
		where titlu_curs = UPPER(titlu_curs);

