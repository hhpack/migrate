/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Event;

use HHPack\Publisher\Message;

use HHPack\Migrate\Database\{QueryResult};

final class MigrationSuccessEvent implements Message {

  public function __construct(
    private string $query,
    private QueryResult $result,
  ) {}

  public function query(): string {
    return $this->query;
  }

  public function startTime(): float {
    return $this->result->startTime();
  }

  public function endTime(): float {
    return $this->result->endTime();
  }

  public function totalTime(): float {
    return $this->endTime() - $this->startTime();
  }

}
