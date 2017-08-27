<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

use HHPack\Migrate\Database\{ QueryProxy, QueryResult };

interface Migration
{
    public function name(): string;
    public function version(): string;
    public function queries(): ImmSet<string>;
    public function change(QueryProxy $proxy): Awaitable<ImmVector<QueryResult>>;
}
