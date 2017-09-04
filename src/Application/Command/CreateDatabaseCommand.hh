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
use HHPack\Migrate\Database\{ Connection, DatabaseClient, DatabaseServer, DatabaseAlreadyExistsException };
use HHPack\Migrate\Database\Query\{ CreateDatabaseQuery, DatabaseAlreadyExistsQuery };
use HHPack\Migrate\Application\{ Context, Command };
use HHPack\Migrate\Application\Configuration\{ Server };
use RuntimeException;

final class CreateDatabaseCommand extends AbstractCommand implements Command
{

    public function __construct()
    {
        $this->usage = "migrate create [OPTIONS]";
        $this->description = "Create a database";
        $this->optionParser = cli\optparser([
            cli\on(['-h', '--help'], 'Display help message', () ==> {
                $this->help = true;
            })
        ]);
    }

    public function run(Context $context): void
    {
        $remainArgs = $this->optionParser->parse($context->cliArgs());

        if ($this->help) {
            $this->displayHelp();
        } else {
            $logger = $context->logger();
            $server = $context->databaseServer();

            $logger->info(sprintf("Create the database %s\n", $server->name()));
            \HH\Asio\join($this->createDatabase($server));
            $logger->info(sprintf("%s is created\n", $server->name()));
        }
    }

    private async function createDatabase(Server $server): Awaitable<void>
    {
        $connection = await $this->connectToServer($server);
        $result = await $connection->query(new DatabaseAlreadyExistsQuery($server->name()));

        if (!$result->isEmpty()) {
            throw new DatabaseAlreadyExistsException(sprintf("Database %s is already exists",
                $server->name()));
        }

        await $connection->query(new CreateDatabaseQuery($server->name()));
    }

    private async function connectToServer(Server $server): Awaitable<Connection>
    {
        $connectServer = new DatabaseServer($server->host(), $server->port());

        try {
            $connectionHandle = await DatabaseClient::createWithoutDbConnection(
                $connectServer,
                $server->user(),
                $server->password()
            );
            return $connectionHandle;
        } catch (\AsyncMysqlConnectException $e) {
            throw new \RuntimeException($e->getMessage(), $e->getCode());
        }
    }

}
