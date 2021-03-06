/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database;

interface Connection {
  public function query(Query $query): Awaitable<QueryResult>;
  public function rawQuery(string $query): Awaitable<QueryResult>;
  public function escapeString(string $value): string;
  public function close(): void;
}
