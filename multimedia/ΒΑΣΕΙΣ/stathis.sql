-- 4396	


-- askhsh 3
-- d erwthma

create table oinopoieio
( epwnimia varchar(50),
  dieuthinsi varchar(50),
  primary key(epwnimia)
);


create table ampelwnas
( epwnimia_a varchar(50),
  perioxi varchar(50),
  id_a int,
  ektasi int not null,
  eidos varchar(50),
  primary key(epwnimia_a, perioxi, id_a),
  foreign key(epwnimia_a) references oinopoieio(epwnimia) on update cascade on delete cascade
);



create table idioktitis
( epwn_id varchar(50),
  idioktitis varchar(50),
  primary key(epwn_id, idioktitis),
  foreign key(epwn_id) references oinopoieio(epwnimia) on update cascade on delete cascade
);
         



create table paragwgi
( epwn_p varchar(50),
  perioxh_p varchar(50),
  id_p int,
  xronia int,
  posothta int,
  hmeromhnia_enarksis date,
  hmeromhnia_liksis date,
  primary key(epwn_p, perioxh_p, id_p, xronia),
  foreign key(perioxh_p) references ampelwnas(perioxi) on update cascade on delete cascade
  foreign key(epwn_p) references ampelwnas(epwnimia_a) on update cascade on delete cascade
  foreign key(id_p) references ampelwnas(id_a) on update cascade on delete cascade
);


-- e erwthma

insert into oinopoieio(epwnimia, dieuthinsi) values('maskarh','kyprou12');
insert into oinopoieio(epwnimia, dieuthinsi) values('kolombou','kozanhs45');
insert into oinopoieio(epwnimia, dieuthinsi) values('malagouzia','kyprou67');

insert into ampelwnas(epwnimia_a, perioxi, id_a, ektasi, eidos) values('maskarh', 'krhth', 4.0, 4567, 'kokkino');
insert into ampelwnas(epwnimia_a, perioxi, id_a, ektasi, eidos) values('mpoutarh', 'thessalonikh', 2.0, 1234, 'leuko');
insert into ampelwnas(epwnimia_a, perioxi, id_a, ektasi, eidos) values('malagoyzia', 'athens', 48.0, 457, 'kokkino');


insert into idioktitis(epwn_id, name_idioktitis) values('mpoutarh', 'kostas papagiannhs');
insert into idioktitis(epwn_id, name_idioktitis) values('tyrnavou', 'spyros papas');
insert into idioktitis(epwn_id, name_idioktitis) values('malagouzia', 'maria giwrgh');


insert into paragwgi(epwn_p, perioxh_p, id_p, xronia, posothta, hmeromhnia_enarksis, hmeromhnia_liksis) values('mpoutarh', 'thessalonikh', 2, 1990, 2, '1990-02-12', '2000-09-23');
insert into paragwgi(epwn_p, perioxh_p, id_p, xronia, posothta, hmeromhnia_enarksis, hmeromhnia_liksis) values('malagoyzia', 'athens', 14, 1995, 23, '1997-02-12', '2002-09-03');
insert into paragwgi(epwn_p, perioxh_p, id_p, xronia, posothta, hmeromhnia_enarksis, hmeromhnia_liksis) values('maskarh', 'larisa', 1, 1997, 43, '1995-06-01', '2012-09-23');



-- st erwthma

delete from ampelwnas where epwnimia_a = 'maskarh' and perioxi = 'krhth' and id_a;
-- ephreazetai o pinakas ampelwnas kai o pinakas paragwgi logo CASCADE




-- z erwthma

update oinopoieio set epwnimia = 'kol' where epwnimia = 'kolombou'
-- ephreazetai o pinakas oinopoieio kai oi pinakew apelwnas, paragwgi logo cascade  
