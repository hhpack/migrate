# migrate

[![Build Status](https://travis-ci.org/hhpack/migrate.svg?branch=master)](https://travis-ci.org/hhpack/migrate)

## Basic usage

First place the **database.json** of the configuration file in the **config** directory.  
Please specify the setting referring to the following.  

* **type** - the type of migration, it only supports **SQL base**.
* **path** - the directory of the migration file.
* **enviroments** - Database connection setting for environment.

```json
{
  "type": "sql",
  "path": "db/migrate",
  "enviroments": {
    "development": {
      "host": "localhost",
      "port": 3306,
      "name": "migrate",
      "user": { "ENV": "DB_USERNAME" },
      "password": { "ENV": "DB_PASSWORD" }
    }
  }
}
```

### Create a database

You can run the create command to create the database.  

```shell
bin/migrate create
```

### Generate a migration file

Use the **gen command** to generate a migration file.

```shell
bin/migrate gen create-users
```

### Upgrade of schema

Use the up command to upgrade the schema.  
You can upgrade to a specific version by specifying the **--to** option.

```shell
bin/migrate up
```

or

```shell
bin/migrate up --to=20150824010439-create-users
```

### Downgrade of schema

To downgrade to the specified version, use the down command.

```shell
bin/migrate down 20150824010439-create-users
```

### Reset of schema

Restore all applied migrations.

```shell
bin/migrate reset
```

### Drop database

You can delete the database with the following command.  

```shell
bin/migrate drop
```


### Migrator

Current version supports SQL based migration.

```hack
use HHPack\Migrate\Migrator;
use HHPack\Migrate\SqlMigrationLoader;
use HHPack\Migrate\DatabaseClient;

$mysql = await DatabaseClient::createConnection('mysql:dbname=migrate;port=3306', 'migrate', 'migrate');
$loader = new SqlMigrationLoader(__DIR__ . '/sql/migrations');

$migrator = new Migrator($loader, $mysql);

await $migrator->upgrade();
```

### Downgrade of schema

```hack
use HHPack\Migrate\Migrator;
use HHPack\Migrate\SqlMigrationLoader;
use HHPack\Migrate\DatabaseClient;

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
