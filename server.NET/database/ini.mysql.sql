CREATE TABLE `users` (
	`guid` CHAR(36) NOT NULL DEFAULT (UUID()),
	`uid` VARCHAR(36) NOT NULL UNIQUE,
	`email` VARCHAR(255) NOT NULL UNIQUE,
	`password` CHAR(60) NOT NULL,
	`active` BOOLEAN NOT NULL DEFAULT true,
	`created_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`last_login_time` TIMESTAMP NOT NULL DEFAULT '2025-01-01 00:00:00',
	`userdata` JSON NOT NULL COMMENT 'user_data_json',
	PRIMARY KEY(`guid`)
);

CREATE UNIQUE INDEX `IDX_guid`
ON `users` (`guid`);
CREATE UNIQUE INDEX `IDX_uid`
ON `users` (`uid`);
CREATE UNIQUE INDEX `IDX_email`
ON `users` (`email`);