/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Logger;

use HHPack\Migrate\{Logger, Output, Console};

final class PlainLogger implements Logger {

  public function __construct(private Output $output = new Console()) {}

  public function debug(string $message): void {
    $this->output->write($message);
  }

  public function info(string $message): void {
    $this->output->write($message);
  }

  public function warn(string $message): void {
    $this->output->write($message);
  }

  public function error(string $message): void {
    $this->output->writeError($message);
  }

}
