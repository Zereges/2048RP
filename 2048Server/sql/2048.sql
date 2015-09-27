/*
SQLyog Community v12.09 (64 bit)
MySQL - 5.5.43-0+deb8u1 : Database - 2048
*********************************************************************
*/


/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `player_data` */

DROP TABLE IF EXISTS `player_data`;

CREATE TABLE `player_data` (
  `id` int(8) NOT NULL COMMENT 'player id (users.id)',
  `data` char(47) DEFAULT '00|00|01|00|00|00|00|00|00|00|01|00|00|00|00|00' COMMENT 'serialized rects data',
  `won` tinyint(1) DEFAULT '0' COMMENT 'won indicator',
  `score` int(8) DEFAULT '0' COMMENT 'score',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `stats_current` */

DROP TABLE IF EXISTS `stats_current`;

CREATE TABLE `stats_current` (
  `player_id` int(8) NOT NULL COMMENT 'player id (users.id)',
  `stats_id` int(8) NOT NULL COMMENT 'stat is (stats_definitions.id)',
  `value` int(20) DEFAULT NULL COMMENT 'value of given stat for given player',
  PRIMARY KEY (`player_id`,`stats_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Table structure for table `stats_definitions` */

DROP TABLE IF EXISTS `stats_definitions`;

CREATE TABLE `stats_definitions` (
  `id` int(8) NOT NULL AUTO_INCREMENT COMMENT 'stat id',
  `name` varchar(32) DEFAULT NULL COMMENT 'stat name',
  `desc` varchar(100) DEFAULT NULL COMMENT 'stat description',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=latin1;

/*Data for the table `stats_definitions` */

insert  into `stats_definitions`(`id`,`name`,`desc`) values (0,'Left moves',NULL),(1,'Right moves',NULL),(2,'Up moves',NULL),(3,'Down moves',NULL),(4,'Total moves',NULL),(5,'Blocks moved',NULL),(6,'Blocks merged',NULL),(7,'Game restarts',NULL),(8,'Games won',NULL),(9,'Games lost',NULL),(10,'Total seconds spent playing',NULL),(11,'Total score gained',NULL),(12,'Highest score obtained',NULL),(13,'Maximal block',NULL),(14,'Slowest win',NULL),(15,'Slowest lose',NULL),(16,'Fastest win',NULL),(17,'Fastest lose',NULL);

/*Table structure for table `stats_global` */

DROP TABLE IF EXISTS `stats_global`;

CREATE TABLE `stats_global` (
  `player_id` int(8) NOT NULL COMMENT 'player id (users.id)',
  `stats_id` int(8) NOT NULL COMMENT 'stat id (stats_definition.id)',
  `value` int(20) DEFAULT NULL COMMENT 'stat value',
  PRIMARY KEY (`player_id`,`stats_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Table structure for table `users` */

DROP TABLE IF EXISTS `users`;

CREATE TABLE `users` (
  `id` int(8) NOT NULL AUTO_INCREMENT COMMENT 'user id',
  `name` varchar(16) NOT NULL COMMENT 'user name',
  `passwd` char(20) NOT NULL COMMENT 'user hashed pass',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
