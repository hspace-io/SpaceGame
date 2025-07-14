DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `idx` int NOT NULL AUTO_INCREMENT,
  `user_id` varchar(30) DEFAULT NULL,
  `password` varchar(150) DEFAULT NULL,
  `cash` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`idx`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `coupons`;
CREATE TABLE `coupons` (
  `idx` int NOT NULL AUTO_INCREMENT,
  `coupon_id` varchar(40) DEFAULT NULL,
  `coupon_name` varchar(30) DEFAULT NULL,
  `user_id` varchar(30) DEFAULT NULL,
  `used` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idx`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DROP TABLE IF EXISTS `toto`;
CREATE TABLE `toto` (
  `idx` int NOT NULL AUTO_INCREMENT,
  `name` varchar(30),
  `bet` bigint NOT NULL,
  PRIMARY KEY (`idx`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `toto` (`name`, `bet`) VALUES ('S!', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('TRUST', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('CyKor', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('KUICS', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('ASC', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('Whois', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('STEALTH', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('GoN', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('PLUS', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('KUality', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('CAT-Security', 10);
INSERT INTO `toto` (`name`, `bet`) VALUES ('SecurityFirst', 10);
