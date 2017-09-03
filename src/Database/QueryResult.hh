<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database;

final class QueryResult
{

    public function __construct(
        private \ConstVector<ImmMap<string, mixed>> $rows,
        private float $startTime,
        private float $endTime
    )
    {
    }

    public function rows(): \ConstVector<ImmMap<string, mixed>>
    {
        return $this->rows;
    }

    public function isEmpty(): bool
    {
        return count($this->rows) <= 0;
    }

    public function startTime(): float
    {
        return $this->startTime;
    }

    public function endTime(): float
    {
        return $this->endTime;
    }

}
