/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database;

use AsyncMysqlClient;
use AsyncMysqlConnection;
use AsyncMysqlQueryResult;

final class MySqlConnection implements Connection {

  public function __construct(private AsyncMysqlConnection $connection) {}

  public function escapeString(string $value): string {
    return $this->connection->escapeString($value);
  }

  public async function query(Query $query): Awaitable<QueryResult> {
    return await $query->execute($this->connection);
  }

  public async function rawQuery(string $query): Awaitable<QueryResult> {
    $result = await $this->connection->query($query);
    return QueryResult::fromAsyncResult($result);
  }

  public function close(): void {
    $this->connection->close();
  }

  /**
   * Create database connection
   */
  public static async function createConnection(
    DSNString $value,
    string $username,
    string $password,
  ): Awaitable<this> {
    $dsn = DataSourceName::fromString($value);

    $connection = await AsyncMysqlClient::connect(
      $dsn->host(),
      $dsn->port(),
      $dsn->name(),
      $username,
      $password,
    );

    return new static($connection);
  }

  /**
   * Create databse server connection
   *
   * NOTE: Right now this does not work with mysqli or PDO connections.
   * @see https://docs.hhvm.com/hack/reference/class/AsyncMysqlClient/adoptConnection/
   */
  public static async function createWithoutDbConnection(
    DatabaseServer $server,
    string $username,
    string $password,
  ): Awaitable<this> {
    $connection = await AsyncMysqlClient::connect(
      $server->host(),
      $server->port(),
      'information_schema',
      $username,
      $password,
    );

    return new static($connection);
  }

}
