# Version 1.0
# Por lo general no se requiere modificar el fichero

export PGDATABASE:=classicmodels
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost

DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore

all: dropdb createdb restore shell
createdb:
	@echo Creando BBDD
	@$(CREATEDB)
dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)
	rm -f *.log
dump:
	@echo creando dumpfile
	@$(PG_DUMP) > $(DBNAME).sql
restore:
	@echo restore data base
	@cat $(DBNAME).sql | $(PSQL)  
psql: shell
shell:
	@echo create psql shell
	@$(PSQL)  

compile: menu.o odbc.o
			$(CC) menu.o odbc.o -lodbc -o menu
			rm *.o
