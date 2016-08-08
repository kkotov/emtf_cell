/* This table will have one line per card type  */
/* Do we need to store whether it's VME or PCI? */
CREATE TABLE CARD_TYPE (                                                       
       TYPE_ID         VARCHAR(512),
       DESCRIPTION     VARCHAR(1024),   /* description of card */
       USERS           VARCHAR(512),    /* list of detectors which use this card */
       CONSTRAINT      CARD_TYPE_PK     PRIMARY KEY (TYPE_ID)               
);                                                                           

/* this table will have one line per actual VME card */
CREATE TABLE VME_CARD (
       SERIAL_NUMBER    VARCHAR(512),
       TYPE_ID		VARCHAR(512),
       CARD_ID		VARCHAR(512),           /* Card identifier from Card Manufactorer */
       BASE_ADDRESS	INTEGER,                /* for VME64x cards should be 0xffffffff (-1) */
       CURRENT_USER     VARCHAR(256),           /* The group which is currently using this card */
       USER_COMMENT     VARCHAR(1024),          /* for example current usage, defects, history */
       LOCATION         VARCHAR(256),           /* Where the card is currently in use, or lab. address */
       CONSTRAINT	VME_CARD_ID_PK		PRIMARY KEY (SERIAL_NUMBER),
       CONSTRAINT	VME_CARD_TYPE_FK	FOREIGN KEY (TYPE_ID)
 						REFERENCES  CARD_TYPE (TYPE_ID)
);

/* this table will have one line per actual PCI card */
CREATE TABLE PCI_CARD (
       SERIAL_NUMBER    VARCHAR(512),
       TYPE_ID		VARCHAR(512),
       CARD_ID		VARCHAR(512),           /* Card identifier from Card Manufactorer */
       VENDOR		INTEGER,                /* The PCI vendor Id code */
       DEVICE		INTEGER,                /* The PCI device Id code */
       CURRENT_USER     VARCHAR(256),           /* The group which is currently using this card */
       USER_COMMENT     VARCHAR(1024),          /* for example current usage, defects, history */
       LOCATION         VARCHAR(256),           /* Where the card is currently in use, or lab. address */
       CONSTRAINT	PCI_CARD_PK		PRIMARY KEY (SERIAL_NUMBER),
       CONSTRAINT	PCI_CARD_TYPE_FK	FOREIGN KEY (TYPE_ID)
						REFERENCES  CARD_TYPE (TYPE_ID)
);

/* This table will have one line per VME address item */
/* Each line refers only to cards of type TYPE_ID */
CREATE TABLE VME_ADDRESS (
       TYPE_ID		VARCHAR(512),
       ITEM_NAME	VARCHAR(512),
       ADDRESS		INTEGER,	       
       MASK		INTEGER,
       ADDRESS_MODIFIER	SMALLINT,
       WIDTH		SMALLINT,	        /* Data width in bytes */
       READ_OR_WRITE	VARCHAR(2),	        /* '','R','W','RW' */
       DESCRIPTION	VARCHAR(1024),
       CONSTRAINT	VME_ADDR_PK		PRIMARY KEY (TYPE_ID, ITEM_NAME),
       CONSTRAINT	VME_ADDR_TYPE_FK	FOREIGN KEY (TYPE_ID)
						REFERENCES  CARD_TYPE (TYPE_ID)
);

/* This table will have one line per VME64x address item */
/* Each line refers only to cards of type TYPE_ID */
CREATE TABLE VME64X_ADDRESS (
       TYPE_ID		VARCHAR(512),
       ITEM_NAME	VARCHAR(512),
       ADDRESS		INTEGER,	       
       SPACE            VARCHAR(16),	        /*'CONFIGURATION', 'MEMORY'*/
       MAP              SMALLINT,               /* 0,1,2,3,4,5,6,7 */
       MASK		INTEGER,
       READ_OR_WRITE	VARCHAR(2),	        /* '','R','W','RW' */
       DESCRIPTION	VARCHAR(1024),
       CONSTRAINT	VME64X_ADDR_PK		PRIMARY KEY (TYPE_ID, ITEM_NAME),
       CONSTRAINT	VME64X_ADDR_TYPE_FK	FOREIGN KEY (TYPE_ID)
						REFERENCES  CARD_TYPE (TYPE_ID)
);

/* This table will have one line per PCI address item */
/* Each line refers only to cards of type TYPE_ID */
CREATE TABLE PCI_ADDRESS (
       TYPE_ID		VARCHAR(512),
       ITEM_NAME	VARCHAR(512),
       ADDRESS		INTEGER,
       MASK		INTEGER,
       SPACE		VARCHAR(16),	        /*'CONFIGURATION', 'MEMORY', 'IO'*/
       BAR		SMALLINT,               /* 0,1,2,3,4,5 */
       READ_OR_WRITE	VARCHAR(2),	        /* '','R','W','RW' */
       DESCRIPTION	VARCHAR(1024),
       CONSTRAINT	PCI_ADDR_PK		PRIMARY KEY (TYPE_ID, ITEM_NAME),
       CONSTRAINT	PCI_ADDR_TYPE_FK	FOREIGN KEY (TYPE_ID)
                                                REFERENCES  CARD_TYPE (TYPE_ID)
);



/***************************************************************************/
/* user info Table                                                         */
/* every line adds an XML attribute to either a line or a column tag.      */
/*   for a line tag give the table name, put a single space into           */
/*     column name and put the user info text into the so called column    */
/*   for a column tag give the table name and the column name and put the  */
/*     the user info text into the so called column                        */ 
/***************************************************************************/
CREATE TABLE table_column_descriptions_ (
        TABLE_NAME			VARCHAR(256),
        COLUMN_NAME			VARCHAR(256),
        USER_INFO			VARCHAR(1024),
	CONSTRAINT TCDESC_PK		PRIMARY KEY (TABLE_NAME,COLUMN_NAME)
);

/***************************************************************************/
/* Version TABLE                                                           */
/* if you want to use versioning of parameters use this table to store     */
/*  the information about the versions                                     */
/***************************************************************************/
CREATE TABLE VERSION (
	VERSION_MAJOR_ID		NUMBER, 
	VERSION_MINOR_ID		NUMBER, 
	VERSION_DATE			DATE, 
	REGISTERED			NUMBER(1) DEFAULT 0,
	CONSTRAINT VERSION_PK		PRIMARY KEY (VERSION_MAJOR_ID, VERSION_MINOR_ID)
);


INSERT INTO table_column_descriptions_
	(TABLE_NAME,	COLUMN_NAME,	USER_INFO)
values	('CARD_TYPE',	' ',		'One row for each card type');


INSERT INTO table_column_descriptions_
	(TABLE_NAME,	COLUMN_NAME,	USER_INFO)
values	('VME_ADDRESS',	'READ_OR_WRITE', 'One of '''',R,W, or RW');


select table_name from user_tables;

commit;
