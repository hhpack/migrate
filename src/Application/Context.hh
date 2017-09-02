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

use HHPack\Migrate\{ Logger };
use HHPack\Migrate\Application\Configuration\{ Configuration, Server };
use HHPack\Migrate\Migration\{ MigrationType, MigrationLoader };
use HHPack\Migrate\Database\{ Connection };

/**
 * Context at the time of execution of migration
 */
interface Context
{
    /**
     * The specified command line argument
     */
    public function cliArgs() : Traversable<string>;


    public function isSqlType() : bool;

    /**
     * Return the migration directory
     */
    public function migrationPath() : string;

    /**
     * Return the logger
     */
    public function logger(): Logger;


    public function databaseServer(): Server;

    /**
     * Return database client to be used for migration
     */
    public function connectDatabase(): Connection;

    /**
     * Return loader to be used for migration
     */
    public function migrationLoader(): MigrationLoader;
}
