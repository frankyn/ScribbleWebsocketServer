DROP TABLE IF EXISTS `apps`;
CREATE TABLE `apps` (
  `id`      int(4) NOT NULL AUTO_INCREMENT,
  `name`    varchar(24),
  `channel` varchar(24),
  `script`  text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=490 DEFAULT CHARSET=latin1;

LOCK TABLES `apps` WRITE;
/*Sample Data*/
/*!40000 ALTER TABLE `apps` DISABLE KEYS */;
INSERT INTO `apps` VALUES (4,'ScribbleChat','scribblechat_channel','ZnVuY3Rpb24gaW5pdCAoICkgXHJcblxyXG5lbmRcclxuXHJcbmZ1bmN0aW9uIG9uQmVhdCAoIClcclxuICAgICByZXR1cm4gXCJcIlxyXG5lbmRcclxuXHJcbmZ1bmN0aW9uIG9uTWVzc2FnZSAoIHVpZCwgbXNnIClcclxuCWJyb2FkY2FzdCAoIHVpZCwgbXNnIClcclxuCXJldHVybiBcIlwiXHJcbmVuZFxyXG5cclxuZnVuY3Rpb24gb25Db25uZWN0ICggdWlkIClcclxuCXJldHVybiBcIlwiXHJcbmVuZFxyXG5cclxuZnVuY3Rpb24gb25EaXNjb25uZWN0ICggdWlkIClcclxuICAgICAgICBicm9hZGNhc3QodWlkLCBcIlVzZXIgZGlzY29ubmVjdGVkXCIpXHJcbglyZXR1cm4gXCJcIlxyXG5lbmQ=');
/*!40000 ALTER TABLE `apps` ENABLE KEYS */;
UNLOCK TABLES;

