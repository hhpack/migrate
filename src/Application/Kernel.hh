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

use HHPack\Migrate\{ Console };
use RuntimeException;

final class Kernel
{
    public function run(Traversable<string> $argv): void
    {
        try {
            (new MigrateApplication(new Console()))->run($argv);
        } catch (RuntimeException $e) {
            echo $e->getMessage();
            exit(-1);
        }
    }
}
