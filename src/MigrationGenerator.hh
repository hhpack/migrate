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

type GenerateVersion = shape(
    "version" => string,
    "name" => string
);

interface MigrationGenerator
{
    public function generate(GenerateVersion $newVersion): void;
}
