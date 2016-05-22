# migrate

[![Build Status](https://travis-ci.org/hhpack/migrate.svg?branch=master)](https://travis-ci.org/hhpack/migrate)

## Basic usage

```hack
use hhpack\migrate\Migrator;
use hhpack\migrate\SqlMigrationLoader;
use hhpack\migrate\DatabaseClient;

$mysql = await DatabaseClient::createConnection('mysql:dbname=migrate;port=3306', 'migrate', 'migrate');
$loader = new SqlMigrationLoader(__DIR__ . '/sql/migrations');

$migrator = new Migrator($loader, $mysql);

await $migrator->upgrade();
```

### Downgrade of schema

```hack
use hhpack\migrate\Migrator;
use hhpack\migrate\SqlMigrationLoader;
use hhpack\migrate\DatabaseClient;

$mysql = await DatabaseClient::createConnection('mysql:dbname=migrate;port=3306', 'migrate', 'migrate');
$loader = new SqlMigrationLoader(__DIR__ . '/sql/migrations');

$migrator = new Migrator($loader, $mysql);

await $migrator->downgrade('20150825102100-create-posts');
```

## Run the test

1. Create a database

		CREATE USER 'migrate'@'localhost' IDENTIFIED BY 'migrate';

2. Create a user

		GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP ON migrate.* TO 'migrate'@'localhost';

3. Execute unit test

	You can run the test with the following command.

		composer install
		composer test
