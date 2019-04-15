/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database\Query;

use HHPack\Migrate\Database\{Query, QueryResult};
use AsyncMysqlConnection;

final class CreateMigrationsTableQuery implements Query {

  public function __construct(private string $tableName) {}

  public async function execute(
    AsyncMysqlConnection $connection,
  ): Awaitable<QueryResult> {
    $result = await $connection->queryf(
      'CREATE TABLE IF NOT EXISTS %T (id int(11) NOT NULL AUTO_INCREMENT, name varchar(255), run_at datetime NOT NULL, PRIMARY KEY (id))',
      $this->tableName,
    );
    return QueryResult::fromAsyncResult($result);
  }

}
