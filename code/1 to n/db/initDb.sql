-- --------------------------------------------------------------------------------------
-- Creating new database
DROP DATABASE IF EXISTS loraiot;
CREATE DATABASE loraiot;

-- Creating new table
USE loraiot;
DROP TABLE IF EXISTS monitoring_data;
CREATE TABLE monitoring_data(
                No_Data INT NOT NULL AUTO_INCREMENT,
                Tanggal CHAR(20),
                Waktu CHAR(10),  
                Suhu1 FLOAT,
                Suhu2 FLOAT,
                Suhu3 FLOAT,
                Suhu4 FLOAT,
                Suhu_Mean FLOAT,
                RH1 FLOAT,
                RH2 FLOAT,
                RH3 FLOAT,
                RH4 FLOAT,
                RH_Mean FLOAT,
                SH1 FLOAT,
                SH2 FLOAT,
                SH3 FLOAT,
                SH4 FLOAT,
                SH_Mean FLOAT,
                IC1 FLOAT,
                IC2 FLOAT,
                IC3 FLOAT,
                IC4 FLOAT,
                IC_Mean FLOAT,
                PRIMARY KEY (No_Data)); 

DROP TABLE IF EXISTS nodes;
CREATE TABLE nodes (
                Node_Id INT NOT NULL,
                Longitude FLOAT,
                Latitude FLOAT,  
				Is_Need_Shade BOOLEAN NOT NULL,
                PRIMARY KEY (Node_Id)); 

-- Creating mean trigger
DELIMITER //

CREATE TRIGGER calculate_suhu_mean_trigger
BEFORE INSERT ON monitoring_data
FOR EACH ROW
BEGIN
  SET NEW.Suhu_Mean = (NEW.Suhu1 + NEW.Suhu2 + NEW.Suhu3 + NEW.Suhu4) / 4;
END//

CREATE TRIGGER calculate_rh_mean_trigger
BEFORE INSERT ON monitoring_data
FOR EACH ROW
BEGIN
  SET NEW.RH_Mean = (NEW.RH1 + NEW.RH2 + NEW.RH3 + NEW.RH4) / 4;
END//

CREATE TRIGGER calculate_sh_mean_trigger
BEFORE INSERT ON monitoring_data
FOR EACH ROW
BEGIN
  SET NEW.SH_Mean = (NEW.SH1 + NEW.SH2 + NEW.SH3 + NEW.SH4) / 4;
END//

CREATE TRIGGER calculate_ic_mean_trigger
BEFORE INSERT ON monitoring_data
FOR EACH ROW
BEGIN
  SET NEW.IC_Mean = (NEW.IC1 + NEW.IC2 + NEW.IC3 + NEW.IC4) / 4;
END//

DELIMITER ;
