-- create table
CREATE TABLE keys (
    tsid TEXT UNIQUE NOT NULL,
    keyi INT NOT NULL
);

-- insert keystroke
INSERT OR IGNORE INTO keys VALUES (strftime('%Y%m%d%H68', 'now', 'localtime'), 0);
UPDATE keys SET keyi = keyi + 1 WHERE tsid = strftime('%Y%m%d%H68', 'now', 'localtime');

INSERT OR IGNORE INTO keys VALUES (strftime('%Y%m%d%H65', 'now', 'localtime'), 0);
UPDATE keys SET keyi = keyi + 1 WHERE tsid = strftime('%Y%m%d%H65', 'now', 'localtime');

-- read keys
-- all
SELECT * FROM keys;
-- in an hour
SELECT * FROM keys WHERE tsid LIKE '2022040315%';
-- in a day
SELECT * FROM keys WHERE tsid LIKE '20220403%';
-- in a month
SELECT * FROM keys WHERE tsid LIKE '202204%';
-- in a year
SELECT * FROM keys WHERE tsid LIKE '2022%';
-- certain keycode
SELECT * FROM keys WHERE tsid LIKE '%68';
-- cerain keycode in span
SELECT * FROM keys WHERE tsid LIKE '202204%68';
-- sum of keys
SELECT SUM(keyi) FROM keys;
-- sum of keys in timespan
SELECT SUM(keyi) FROM keys WHERE tsid LIKE '202204%';