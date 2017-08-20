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

use SplFileObject;

final class Console implements Output
{

    public function __construct(
        private SplFileObject $stdout = new SplFileObject('php://stdout'),
        private SplFileObject $stderr = new SplFileObject('php://stderr')
    )
    {
    }

    // TODO check all content write to stream
    public function write(string $message): void
    {
        $this->stdout->fwrite($message, strlen($message));
    }

    // TODO check all content write to stream
    public function writeError(string $message): void
    {
        $this->stderr->fwrite($message, strlen($message));
    }

}
