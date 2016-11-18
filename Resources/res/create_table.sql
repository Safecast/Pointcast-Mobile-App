DROP TABLE IF EXISTS l_measurements_history;
CREATE TABLE IF NOT EXISTS chart_item (
  device_id int,
  timestamp bigint(20) NOT NULL,
  value int(11) DEFAULT NULL
) ;
