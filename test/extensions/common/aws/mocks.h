#pragma once

#include "envoy/http/message.h"

#include "source/common/http/message_impl.h"
#include "source/extensions/common/aws/aws_cluster_manager.h"
#include "source/extensions/common/aws/credentials_provider.h"
#include "source/extensions/common/aws/credentials_provider_impl.h"
#include "source/extensions/common/aws/metadata_fetcher.h"
#include "source/extensions/common/aws/signer.h"

#include "test/mocks/upstream/cluster_manager.h"

#include "gmock/gmock.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Aws {

class MockMetadataFetcher : public MetadataFetcher {
public:
  MOCK_METHOD(void, cancel, ());
  MOCK_METHOD(absl::string_view, failureToString, (MetadataFetcher::MetadataReceiver::Failure));
  MOCK_METHOD(void, fetch,
              (Http::RequestMessage & message, Tracing::Span& parent_span,
               MetadataFetcher::MetadataReceiver& receiver));
};

class MockMetadataReceiver : public MetadataFetcher::MetadataReceiver {
public:
  MOCK_METHOD(void, onMetadataSuccess, (const std::string&& body));
  MOCK_METHOD(void, onMetadataError, (MetadataFetcher::MetadataReceiver::Failure reason));
};

class MockCredentialsProvider : public CredentialsProvider {
public:
  MockCredentialsProvider();
  ~MockCredentialsProvider() override;

  MOCK_METHOD(Credentials, getCredentials, ());
  MOCK_METHOD(bool, credentialsPending, ());
  MOCK_METHOD(std::string, providerName, ());
};

class MockSigner : public Signer {
public:
  MockSigner();
  ~MockSigner() override;

  MOCK_METHOD(absl::Status, sign, (Http::RequestMessage&, bool, absl::string_view));
  MOCK_METHOD(absl::Status, sign, (Http::RequestHeaderMap&, const std::string&, absl::string_view));
  MOCK_METHOD(absl::Status, signEmptyPayload, (Http::RequestHeaderMap&, absl::string_view));
  MOCK_METHOD(absl::Status, signUnsignedPayload, (Http::RequestHeaderMap&, absl::string_view));
  MOCK_METHOD(bool, addCallbackIfCredentialsPending, (CredentialsPendingCallback &&));
};

class MockFetchMetadata {
public:
  virtual ~MockFetchMetadata() = default;

  MOCK_METHOD(absl::optional<std::string>, fetch, (Http::RequestMessage&), (const));
};

class DummyMetadataFetcher {
public:
  absl::optional<std::string> operator()(Http::RequestMessage&) { return absl::nullopt; }
};

class MockAwsClusterManager : public AwsClusterManager {
public:
  ~MockAwsClusterManager() override = default;

  MOCK_METHOD(absl::Status, addManagedCluster,
              (absl::string_view cluster_name,
               const envoy::config::cluster::v3::Cluster::DiscoveryType cluster_type,
               absl::string_view uri));

  MOCK_METHOD(absl::StatusOr<AwsManagedClusterUpdateCallbacksHandlePtr>,
              addManagedClusterUpdateCallbacks,
              (absl::string_view cluster_name, AwsManagedClusterUpdateCallbacks& cb));
  MOCK_METHOD(absl::StatusOr<std::string>, getUriFromClusterName, (absl::string_view cluster_name));
  MOCK_METHOD(void, createQueuedClusters, ());
};

class MockAwsManagedClusterUpdateCallbacks : public AwsManagedClusterUpdateCallbacks {
public:
  MOCK_METHOD(void, onClusterAddOrUpdate, ());
};

class MockCredentialsProviderChain : public CredentialsProviderChain {
public:
  MOCK_METHOD(Credentials, chainGetCredentials, ());
  MOCK_METHOD(bool, addCallbackIfChainCredentialsPending, (CredentialsPendingCallback &&));
  MOCK_METHOD(void, onCredentialUpdate, ());
};

} // namespace Aws
} // namespace Common
} // namespace Extensions
} // namespace Envoy
