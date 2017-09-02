<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application;

use HHPack\Migrate\{ Logger };
use HHPack\Migrate\Migration\{ MigrationLoader };
use HHPack\Migrate\Migration\Loader\{ SqlMigrationLoader };
use HHPack\Migrate\Database\{ Connection, DatabaseClient, DatabaseServer };
use HHPack\Migrate\Application\Configuration\{ Configuration, Server };

final class MigrateContext implements Context
{

    public function __construct(
        private Traversable<string> $args,
        private Configuration $config,
        private Logger $logger
    )
    {
    }

    public function cliArgs() : Traversable<string>
    {
        return $this->args;
    }

    public function isSqlType() : bool
    {
        return $this->config->isSqlType();
    }

    public function migrationPath() : string
    {
        return $this->config->migrationPath();
    }

    public function logger(): Logger
    {
        return $this->logger;
    }

    public function databaseServer(): Server
    {
        return $this->config->databaseServer();
    }

    public function connectDatabase() : Connection
    {
        try {
            $connectionHandle = DatabaseClient::createConnection(
                $this->config->databaseDSN(),
                $this->config->databaseUser(),
                $this->config->databasePassword()
            );
            return \HH\Asio\join($connectionHandle);
        } catch (\AsyncMysqlConnectException $e) {
            throw new \RuntimeException($e->getMessage(), $e->getCode());
        }
    }

    public function migrationLoader() : MigrationLoader
    {
        $path = $this->config->migrationPath();

        if ($this->config->isSqlType()) {
            $loader = new SqlMigrationLoader($path);
        } else {
            $loader = new SqlMigrationLoader($path);
        }

        return $loader;
    }

}
