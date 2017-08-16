<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application;

use HHPack\Migrate\{ Connection, MigrationLoader };

/**
 * Context at the time of execution of migration
 */
interface Context
{
    /**
     * The specified command line argument
     */
    public function cliArgs() : Traversable<string>;

    /**
     * Return database client to be used for migration
     */
    public function databaseClient(): Awaitable<Connection>;

    /**
     * Return loader to be used for migration
     */
    public function migrationLoader(): MigrationLoader;
}
