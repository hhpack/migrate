<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Configuration;

use HHPack\Migrate\Migration\{ MigrationType };

type MigrationLoader = shape(
    "type" => MigrationType,
    "path" => string
);

type DatabaseServer = shape(
    "host" => string,
    "port" => int,
    "name" => string,
    "user" => string,
    "password" => string
);

type DatabaseEnviroment = ImmMap<string, DatabaseServer>;
