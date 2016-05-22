<?hh //partial

namespace hhpack\migrate\spec;

use hhpack\migrate\DataSourceName;

describe(DataSourceName::class, function() {
  beforeEach(function() {
    $this->dns = DataSourceName::fromString('mysql:dbname=test;port=3306');
  });
  describe('#port', function() {
    it('return port number', function () {
      expect($this->dns->port())->toBe(3306);
    });
  });
  describe('#type', function() {
    it('return db type', function () {
      expect($this->dns->type())->toBe('mysql');
    });
  });
  describe('#name', function() {
    it('return db name', function () {
      expect($this->dns->name())->toBe('test');
    });
  });
});
