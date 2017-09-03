<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Command;

use HHPack\Getopt as cli;
use HHPack\Getopt\Parser\{ OptionParser };
use HHPack\Migrate\{ Migrator };
use HHPack\Migrate\Migration\{ MigrationLoader };
use HHPack\Migrate\Migration\Loader\{ SqlMigrationLoader };
use HHPack\Migrate\Database\{ Connection, DatabaseClient };
use HHPack\Migrate\Application\{ Context, Command };
use HHPack\Migrate\Application\Configuration\{ Migration, Server };
use RuntimeException;


abstract class MigrateSchemaCommand extends AbstractCommand implements Command
{

    protected function createMigrator(Context $context): Migrator
    {
        $server = $context->databaseServer();
        $connection = $this->connectToServer($server);

        $loader = $this->loaderFrom($context->migration());

        return new Migrator(
            $loader,
            $connection,
            $context->logger()
        );
    }

    private function loaderFrom(Migration $setting) : MigrationLoader
    {
        return new SqlMigrationLoader($setting->path());
    }

    private function connectToServer(Server $server): Connection
    {
        try {
            $connectionHandle = DatabaseClient::createConnection(
                $server->dns(),
                $server->user(),
                $server->password()
            );
            return \HH\Asio\join($connectionHandle);
        } catch (\AsyncMysqlConnectException $e) {
            throw new \RuntimeException($e->getMessage(), $e->getCode());
        }
    }

}
