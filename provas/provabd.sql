-- CREATE TABLES
CREATE TABLE maquina(
  codmaq SERIAL,
  nomemaq TEXT,
  CONSTRAINT pk_maquina PRIMARY KEY (codmaq)
);
  
CREATE TABLE produto(
  codprod SERIAL,
  descrprod TEXT,
  pesoprod FLOAT,
  CONSTRAINT pk_produto PRIMARY KEY (codprod)
);

CREATE TABLE maqprod(
  codmaq INTEGER,
  codprod INTEGER,
  tempo float,
  CONSTRAINT pk_maquinaprod PRIMARY KEY (codmaq, codprod),
  CONSTRAINT fk_maquinaprod_maquina FOREIGN KEY (codmaq) REFERENCES maquina(codmaq),
  CONSTRAINT fk_maquinaprod_produto FOREIGN KEY (codmaq) REFERENCES produto(codprod)
);

CREATE TABLE composprod (
  codprodcomposto INTEGER,
  codprodcomponente INTEGER,
  CONSTRAINT pk_codprodcomposto PRIMARY KEY (codprodcomposto, codprodcomponente),
  CONSTRAINT pk_composprod_composto FOREIGN KEY (codprodcomposto) REFERENCES produto(codprod),
  CONSTRAINT pk_composprod_componente FOREIGN KEY (codprodcomponente) REFERENCES produto(codprod)
);


