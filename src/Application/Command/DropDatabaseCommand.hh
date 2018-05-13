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
use HHPack\Getopt\Parser\{OptionParser};
use HHPack\Migrate\Database\{
  Connection,
  DatabaseClient,
  DatabaseServer,
  DatabaseNotFoundException
};
use HHPack\Migrate\Database\Query\{
  DropDatabaseQuery,
  DatabaseAlreadyExistsQuery
};
use HHPack\Migrate\Application\{Context, Command};
use HHPack\Migrate\Application\Configuration\{Server};
use RuntimeException;

final class DropDatabaseCommand extends DatabaseCommand implements Command {

  public function __construct() {
    $this->usage = "migrate drop [OPTIONS]";
    $this->description = "Drop a database";
    $this->optionParser = cli\optparser(
      [
        cli\on(
          ['-h', '--help'],
          'Display help message',
          () ==> {
            $this->help = true;
          },
        ),
      ],
    );
  }

  public function run(Context $context): void {
    $remainArgs = $this->optionParser->parse($context->cliArgs());

    if ($this->help) {
      $this->displayHelp();
    } else {
      $logger = $context->logger();
      $server = $context->databaseServer();

      $logger->info(\sprintf("Drop the database %s\n", $server->name()));
      \HH\Asio\join($this->dropDatabase($server));
      $logger->info(\sprintf("%s is droped\n", $server->name()));
    }
  }

  private async function dropDatabase(Server $server): Awaitable<void> {
    $connection = await $this->connectToServer($server);
    $result = await $connection->query(
      new DatabaseAlreadyExistsQuery($server->name()),
    );

    if ($result->isEmpty()) {
      throw new DatabaseNotFoundException(
        \sprintf("Database %s is not found", $server->name()),
      );
    }

    await $connection->query(new DropDatabaseQuery($server->name()));
  }

}
