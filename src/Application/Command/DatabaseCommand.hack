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
use HHPack\Migrate\Database\{Connection, DatabaseServer, DatabaseClient};
use HHPack\Migrate\Application\{Command};
use HHPack\Migrate\Application\Configuration\{Server};
use RuntimeException;

abstract class DatabaseCommand extends AbstractCommand implements Command {

  protected async function connectToServer(
    Server $server,
  ): Awaitable<Connection> {
    $connectServer = new DatabaseServer($server->host(), $server->port());

    try {
      $connectionHandle = await DatabaseClient::createWithoutDbConnection(
        $connectServer,
        $server->user(),
        $server->password(),
      );
      return $connectionHandle;
    } catch (\AsyncMysqlConnectException $e) {
      throw new \RuntimeException($e->getMessage(), $e->getCode());
    }
  }

}
