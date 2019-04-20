/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Migration\Type;

use HHPack\Migrate\{File, Migration, FileNotFoundException};
use HHPack\Migrate\Database\{QueryProxy, QueryResult};

final class SqlMigration implements Migration {

  private ImmSet<string> $queries;

  public function __construct(
    private string $name,
    Traversable<string> $queries,
  ) {
    $this->queries = ImmSet::fromItems($queries);
  }

  public function name(): string {
    return $this->name;
  }

  public function version(): string {
    return \preg_replace("/^(\d+)\-.+/", "$1", $this->name);
  }

  public function queries(): ImmSet<string> {
    return $this->queries;
  }

  public async function change(
    QueryProxy $proxy,
  ): Awaitable<ImmVector<QueryResult>> {
    $results = Vector {};

    foreach ($this->queries as $query) {
      $result = await $proxy->query($query);
      $results->add($result);
    }

    return $results->toImmVector();
  }

  public static function fromFile(string $file): this {
    if (!\file_exists($file)) {
      throw new FileNotFoundException("$file is not found");
    }

    $absolutePath = File\absolute_path($file);

    $name = \preg_replace("/\.(down|up)\..+$/", "", \basename($absolutePath));
    $sql = \file_get_contents($absolutePath);

    $queries = \explode(';', \trim($sql));
    $queries = \array_slice($queries, 0, \count($queries) - 1);

    return new SqlMigration($name, $queries);
  }

}
