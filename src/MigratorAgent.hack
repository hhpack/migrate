/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

use HHPack\Migrate\Database\{Connection, QueryProxy, QueryResult};
use HHPack\Migrate\Event\{EventPublisher};

final class MigratorAgent implements QueryProxy {

  public function __construct(
    private Connection $connection,
    private EventPublisher $publisher,
  ) {}

  public async function query(string $query): Awaitable<QueryResult> {
    await $this->publisher->migrationStart($query);
    $result = await $this->connection->rawQuery($query);
    await $this->publisher->migrationSuccess($query, $result);

    return $result;
  }

}
