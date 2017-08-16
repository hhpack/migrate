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

use HHPack\Migrate\{ Connection, DatabaseClient, MigrationLoader, SqlMigrationLoader };
use HHPack\Migrate\Application\Configuration\{ Configuration };

final class MigrateContext implements Context
{

    public function __construct(
        private Traversable<string> $args,
        private Configuration $config
    )
    {
    }

    public function cliArgs() : Traversable<string>
    {
        return $this->args;
    }

    public async function databaseClient() : Awaitable<Connection>
    {
        return await DatabaseClient::createConnection(
            $this->config->databaseDSN(),
            $this->config->databaseUser(),
            $this->config->databasePassword()
        );
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
